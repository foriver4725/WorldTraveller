#include "Home/UI/StartGameUiManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"

AStartGameUiManager::AStartGameUiManager() : Super()
{
	SetUiEnabled(false);
}

void AStartGameUiManager::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(widgetClass))
	{
		userWidget = CreateWidget<UUserWidget>(GetWorld(), widgetClass);
		if (userWidget)
		{
			submitButton = Cast<UButton>(userWidget->GetWidgetFromName(TEXT("SubmitButton")));
			if (submitButton)
				submitButton->OnClicked.AddUniqueDynamic(this, &AStartGameUiManager::OnSubmitButtonClicked);

			userWidget->AddToViewport();
		}
	}
}

void AStartGameUiManager::OnSubmitButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Submit Button Clicked!"));
}

bool AStartGameUiManager::GetUiEnabled() const
{
	if (!IsValid(userWidget)) return false;
	return userWidget->bIsEnabled;
}

void AStartGameUiManager::SetUiEnabled(bool bEnabled)
{
	if (!IsValid(userWidget)) return;
	userWidget->SetIsEnabled(bEnabled);
}
