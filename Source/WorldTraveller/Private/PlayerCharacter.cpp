﻿#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputManager.h"
#include "UiManager.h"
#include "Enums/UiDescriptionTextType.h"
#include "Enums/UiType.h"

using DescTextType = EUiDescriptionTextType;
using UiType = EUiType;

APlayerCharacter::APlayerCharacter() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	// コリジョンカプセルのサイズを設定
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// カメラコンポーネントを作成
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(GetCapsuleComponent());
	camera->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	camera->bUsePawnControlRotation = true;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> uiManagers;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UUiManager::StaticClass(), uiManagers);
	if (uiManagers.Num() == 1)
		uiManager = uiManagers[0];  // レベル内に一つのみ存在するはず

	GetCharacterMovement()->SetWalkableFloorAngle(slopLimit);
	GetCharacterMovement()->JumpZVelocity = jumpZVelocity * jumpZVelocityMultiplier;
}

void APlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	using Subsystem = UEnhancedInputLocalPlayerSubsystem;

	if (TObjectPtr<APlayerController> playerController = Cast<APlayerController>(Controller))
		if (TObjectPtr<Subsystem> subsystem = ULocalPlayer::GetSubsystem<Subsystem>(playerController->GetLocalPlayer()))
			subsystem->AddMappingContext(mappingContext, 0);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		inputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		inputComponent->BindAction(lookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		inputComponent->BindAction(jumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		inputComponent->BindAction(jumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		inputComponent->BindAction(interactAction, ETriggerEvent::Started, this, &APlayerCharacter::OnInteractedPressed);
		inputComponent->BindAction(cancelAction, ETriggerEvent::Started, this, &APlayerCharacter::OnCancelPressed);
		inputComponent->BindAction(escapeAction, ETriggerEvent::Started, this, &APlayerCharacter::OnEscapePressed);
		inputComponent->BindAction(escapeAction, ETriggerEvent::Completed, this, &APlayerCharacter::OnEscapeReleased);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	{
		FName outTag = "";
		bClickable = CheckClickableRay(outTag);
		clickableTag = outTag;

		SetDispCanClick(bClickable);

		if (AInputManager* inputManager = AInputManager::Instance())
			if (inputManager->GetInteractPressed())
				this->OnInteractedAgainstClickables();
	}
}

void APlayerCharacter::OnInteractedAgainstClickables()
{
	if (!bClickable) return;

	static const FName Home_PedestalTag = TEXT("Pedestal");

	if (clickableTag == Home_PedestalTag)
	{
		if (IUiManager* iUiManager = Cast<IUiManager>(GetValid(uiManager)))
			iUiManager->SetUiEnabled(UiType::Home_StartGame, true);
	}
	else;
}

void APlayerCharacter::OnInteractedPressed(const FInputActionValue& value)
{
	if (AInputManager* inputManager = AInputManager::Instance())
		inputManager->InvokeInteractPressed();
}

void APlayerCharacter::OnCancelPressed(const FInputActionValue& value)
{
	if (AInputManager* inputManager = AInputManager::Instance())
		inputManager->InvokeCancelPressed();
}

void APlayerCharacter::OnEscapePressed(const FInputActionValue& value)
{
	if (AInputManager* inputManager = AInputManager::Instance())
		inputManager->InvokeEscapePressed();
}

void APlayerCharacter::OnEscapeReleased(const FInputActionValue& value)
{
	if (AInputManager* inputManager = AInputManager::Instance())
		inputManager->InvokeEscapeReleased();
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	FVector2D movementVector = value.Get<FVector2D>() * speedMultiplier;

	if (IsValid(Controller))
	{
		AddMovementInput(GetActorForwardVector(), movementVector.Y);
		AddMovementInput(GetActorRightVector(), movementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& value)
{
	FVector2D lookAxisVector = value.Get<FVector2D>() * lookSensitivityMultiplier;

	if (IsValid(Controller))
	{
		AddControllerYawInput(lookAxisVector.X);
		AddControllerPitchInput(lookAxisVector.Y);
	}
}

// カメラの正面にレイを飛ばし、有効なアクタに当たったかどうか返す
// 有効なアクタに当たったなら、その識別タグ(インデックス1番目のもの)も返す
bool APlayerCharacter::CheckClickableRay(FName& outTag)
{
	static const FName CanClickTag = TEXT("CanClick");

	outTag = "";

	if (!IsValid(camera))
		return false;

	FHitResult hitResult;
	FVector start = camera->GetComponentLocation();
	FVector end = start + camera->GetForwardVector() * clickableRayMaxDistance;
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);

	if (!GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, collisionParams))
		return false;  // 何にも当たらなかった

	TObjectPtr<AActor> hitActor = hitResult.GetActor();
	if (!IsValid(hitActor))
		return false;  // アクタに当たらなかった

	if (hitActor->ActorHasTag(CanClickTag))
	{
		// 有効なアクタに当たった!

		const TArray<FName>& tags = hitActor->Tags;
		if (tags.Num() <= 1)
			return false; // 識別タグを持っていなかった

		outTag = tags[1];
		return true;
	}

	return false;  // 有効なアクタに当たらなかった
}

void APlayerCharacter::SetDispCanClick(bool bEnabled)
{
	if (IsValid(uiManager))
	{
		if (IUiManager* iUiManager = Cast<IUiManager>(uiManager))
		{
			iUiManager->SetPointerActivation(bEnabled);
			iUiManager->SetDescriptionText(bEnabled ? DescTextType::CanClick : DescTextType::None);
		}
	}
}

void APlayerCharacter::RandomizeJumpZVelocityMultiplier(const FRandomStream& rand)
{
	jumpZVelocityMultiplier = rand.FRandRange(jumpZVelocityMultiplierOnRandomizedMin, jumpZVelocityMultiplierOnRandomizedMax);
	GetCharacterMovement()->JumpZVelocity = jumpZVelocity * jumpZVelocityMultiplier;
}

void APlayerCharacter::SetInputEnabled(bool bEnabled)
{
	if (APlayerController* pc = Cast<APlayerController>(GetController()))
	{
		if (bEnabled) EnableInput(pc);
		else DisableInput(pc);
	}
}
