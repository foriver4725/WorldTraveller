#include "Home/UI/Home_StartGameUiHandler.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"

AHome_StartGameUiHandler::AHome_StartGameUiHandler() : Super()
{
}

void AHome_StartGameUiHandler::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(widgetClass))
	{
		userWidget = CreateWidget<UUserWidget>(GetWorld(), widgetClass);
		if (userWidget)
		{
			SetUiEnabled(false);

			submitButton = Cast<UButton>(userWidget->GetWidgetFromName(TEXT("SubmitButton")));
			if (submitButton)
				submitButton->OnClicked.AddUniqueDynamic(this, &AHome_StartGameUiHandler::OnSubmitButtonClicked);

			userWidget->AddToViewport();
		}
	}
}

void AHome_StartGameUiHandler::OnSubmitButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Submit Button Clicked!"));
}

bool AHome_StartGameUiHandler::GetUiEnabled() const
{
	return enabled;
}

void AHome_StartGameUiHandler::SetUiEnabled(bool bEnabled)
{
	if (!IsValid(userWidget)) return;

	enabled = bEnabled;
	userWidget->SetIsEnabled(bEnabled);
	userWidget->SetRenderOpacity(bEnabled ? 1 : 0);
}
