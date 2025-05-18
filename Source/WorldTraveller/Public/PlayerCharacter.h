#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class UInputComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class WORLDTRAVELLER_API APlayerCharacter final : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	void RandomizeJumpZVelocityMultiplier();

protected:
	virtual void BeginPlay() override final;
	virtual void NotifyControllerChanged() override final;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override final;
	virtual void Tick(float DeltaTime) override final;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Property|Input")
	TObjectPtr<UInputMappingContext> mappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Input")
	TObjectPtr<UInputAction> submitAction;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Input")
	TObjectPtr<UInputAction> cancelAction;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Input")
	TObjectPtr<UInputAction> jumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Input")
	TObjectPtr<UInputAction> moveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Input")
	TObjectPtr<UInputAction> lookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "10.0", ClampMax = "80.0"))
	float slopLimit = 45.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "20.0"))
	float speedMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "2000.0"))
	float jumpZVelocity = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "20.0"))
	float lookSensitivityMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "500.0"))
	float clickableRayMaxDistance = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float jumpZVelocityMultiplierMin = 0.8f;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float jumpZVelocityMultiplierMax = 3.0f;

	UPROPERTY() TObjectPtr<UCameraComponent> camera = nullptr;
	UPROPERTY() TObjectPtr<AActor> uiManager = nullptr;

	float jumpZVelocityMultiplier = 1.0f;
	bool bClickable = false;
	FName clickableTag = "";

	void OnSubmit();
	void OnCancel();
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

	bool CheckClickableRay(FName& outTag);
	void SetDispCanClick(bool bEnabled);
};
