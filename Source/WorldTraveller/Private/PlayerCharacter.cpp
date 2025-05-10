#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Home/UiManager_Home.h"
#include "EDescriptionText.h"

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

	uiManager = Cast<AUiManager_Home>(UGameplayStatics::GetActorOfClass(GetWorld(), AUiManager_Home::StaticClass()));
}

void APlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(mappingContext, 0);
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		inputComponent->BindAction(submitAction, ETriggerEvent::Started, this, &APlayerCharacter::OnSubmit);
		inputComponent->BindAction(cancelAction, ETriggerEvent::Started, this, &APlayerCharacter::OnCancel);
		inputComponent->BindAction(jumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		inputComponent->BindAction(jumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		inputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		inputComponent->BindAction(lookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckClickableRay();
}

void APlayerCharacter::OnSubmit()
{
}

void APlayerCharacter::OnCancel()
{
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	FVector2D movementVector = value.Get<FVector2D>();

	if (IsValid(Controller))
	{
		AddMovementInput(GetActorForwardVector(), movementVector.Y);
		AddMovementInput(GetActorRightVector(), movementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& value)
{
	FVector2D lookAxisVector = value.Get<FVector2D>();

	if (IsValid(Controller))
	{
		AddControllerYawInput(lookAxisVector.X);
		AddControllerPitchInput(lookAxisVector.Y);
	}
}

// カメラの正面にレイを飛ばし、クリックできるものに当たったらポインターをアクティブにする
void APlayerCharacter::CheckClickableRay()
{
	static const TArray<FName> CanClickTags
	{
		TEXT("CanClick"),
	};

	FHitResult hitResult;
	FVector start = camera->GetComponentLocation();
	FVector end = start + camera->GetForwardVector() * clickableRayMaxDistance;
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);

	if (!GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, collisionParams))
	{
		// 何にも当たらなかった
		FromClickableRayResult(false);
		return;
	}

	AActor* hitActor = hitResult.GetActor();
	if (!hitActor)
	{
		// アクタに当たらなかった
		FromClickableRayResult(false);
		return;
	}

	for (const auto& tag : CanClickTags)
	{
		if (hitActor->ActorHasTag(tag))
		{
			// 有効なアクタに当たった!
			FromClickableRayResult(true);
			return;
		}
	}

	// 有効なアクタに当たらなかった
	FromClickableRayResult(false);
	return;
}

void APlayerCharacter::FromClickableRayResult(bool bSucceeded)
{
	if (IsValid(uiManager))
	{
		uiManager->SetPointerActivation(bSucceeded);
		uiManager->SetDescriptionText(bSucceeded ? EDescriptionText::CanClick : EDescriptionText::None);
	}
}
