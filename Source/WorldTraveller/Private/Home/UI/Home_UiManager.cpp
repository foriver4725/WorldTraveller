#include "Home/UI/Home_UiManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Home/UI/Home_StartGameUiHandler.h"
#include "Home/UI/Home_UiType.h"
#include "Home/UI/Home_UiDescriptionTextType.h"

using DescTextType = EHome_UiDescriptionTextType;
using UiType = EHome_UiType;

AHome_UiManager::AHome_UiManager() : Super()
{
	currentDescriptionText = DescTextType::None;
}

void AHome_UiManager::BeginPlay()
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

void AHome_UiManager::SetPointerActivation(bool bActivate)
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

void AHome_UiManager::SetDescriptionText(DescTextType textType)
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
		case DescTextType::None:
			descriptionText->SetText(NoneText);
			break;
		case DescTextType::CanClick:
			descriptionText->SetText(CanClickText);
			break;
		default:
			break;
		}
	}
}

void AHome_UiManager::SetUiEnabled(UiType type, bool bEnabled)
{
	AActor* uiHandlerActor = nullptr;
	switch (type)
	{
	case UiType::StartGame:
		uiHandlerActor = startGameUiHandler;
		break;
	default:
		break;
	}

	if (!IsValid(uiHandlerActor)) return;

	if (IHome_UiHandler* uiHandler = Cast<IHome_UiHandler>(uiHandlerActor))
		uiHandler->SetUiEnabled(bEnabled);
}
