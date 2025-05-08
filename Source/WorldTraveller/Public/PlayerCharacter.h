#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UInputComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class WORLDTRAVELLER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override final;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override final;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Property|Input")
	UInputMappingContext* mappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Input")
	UInputAction* submitAction;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Input")
	UInputAction* cancelAction;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Input")
	UInputAction* jumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Input")
	UInputAction* moveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Property|Input")
	UInputAction* lookAction;

	void Move(const FInputActionValue& value);

	void Look(const FInputActionValue& value);
};
