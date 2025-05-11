#include "Home/UiManager_Home.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Home/UI/EDescriptionText.h"
#include "Home/UI/EUiType.h"
#include "Home/UI/IUiManager.h"
#include "Home/UI/StartGameUiManager.h"

using namespace NHome;

AUiManager_Home::AUiManager_Home() : Super()
{
	currentDescriptionText = EDescriptionText::None;
}

void AUiManager_Home::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(widgetClass))
	{
		userWidget = CreateWidget<UUserWidget>(GetWorld(), widgetClass);
		if (userWidget)
		{
			pointer = Cast<UImage>(userWidget->GetWidgetFromName(TEXT("Pointer")));
			descriptionText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("DescriptionText")));

			userWidget->AddToViewport();
		}
	}
}

void AUiManager_Home::SetPointerActivation(bool bActivate)
{
	if (bIsPointerActive == bActivate)
		return;
	bIsPointerActive = bActivate;

	if (IsValid(pointer))
	{
		pointer->SetColorAndOpacity(bActivate ? FLinearColor::Red : FLinearColor::White);
		pointer->SetBrushSize(bActivate ? FVector2D(16) : FVector2D(8));
	}
}

void AUiManager_Home::SetDescriptionText(EDescriptionText textType)
{
	static const FText NoneText = FText::GetEmpty();
	static const FText CanClickText = FText::FromString(TEXT("Click or A"));

	if (currentDescriptionText == textType)
		return;
	currentDescriptionText = textType;

	if (IsValid(descriptionText))
	{
		switch (textType)
		{
		case EDescriptionText::None:
			descriptionText->SetText(NoneText);
			break;
		case EDescriptionText::CanClick:
			descriptionText->SetText(CanClickText);
			break;
		default:
			break;
		}
	}
}

void AUiManager_Home::SetUiEnabled(EUiType type, bool bEnabled)
{
	IUiManager* uiManager = nullptr;
	switch (type)
	{
	case EUiType::StartGame:
		uiManager = startGameUiManager;
		break;
	default:
		break;
	}

	if (!IsValid(Cast<AActor>(uiManager))) return;
	uiManager->SetUiEnabled(bEnabled);
}
