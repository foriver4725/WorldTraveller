#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class UInputComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class AUiManager_Home;

UCLASS()
class WORLDTRAVELLER_API APlayerCharacter final : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

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

	UPROPERTY(EditDefaultsOnly, Category = "Property|Value", meta = (ClampMin = "0.0", ClampMax = "500.0"))
	float clickableRayMaxDistance;

	TObjectPtr<UCameraComponent> camera = nullptr;
	TObjectPtr<AUiManager_Home> uiManager = nullptr;

	void OnSubmit();
	void OnCancel();
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

	void CheckClickableRay();
	void FromClickableRayResult(bool bSucceeded);
};
