#include "Main/UI/Main_UiManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UiHandler.h"
#include "UiZOrders.h"
#include "Enums/UiType.h"
#include "Enums/UiDescriptionTextType.h"

using DescTextType = EUiDescriptionTextType;
using UiType = EUiType;

AMain_UiManager::AMain_UiManager()
{
	currentDescriptionText = DescTextType::None;
}

void AMain_UiManager::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(widgetClass))
	{
		userWidget = CreateWidget<UUserWidget>(GetWorld(), widgetClass);
		if (IsValid(userWidget))
		{
			pointer = Cast<UImage>(userWidget->GetWidgetFromName(TEXT("Pointer")));
			descriptionText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("DescriptionText")));
			coordinatesText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("CoordinatesText")));

			userWidget->AddToViewport(FUiZOrders::Main_General);
		}
	}
}

void AMain_UiManager::SetPointerActivation(bool bActivate)
{
	if (bIsPointerActive == bActivate)
		return;
	bIsPointerActive = bActivate;

	if (IsValid(pointer))
	{
		pointer->SetColorAndOpacity(bActivate ? FLinearColor::Red : FLinearColor::White);
		pointer->SetDesiredSizeOverride(bActivate ? FVector2D(16) : FVector2D(8));
	}
}

void AMain_UiManager::SetDescriptionText(DescTextType textType)
{
	static const FText NoneText = FText::GetEmpty();
	static const FText CanClickText = FText::FromString(TEXT("Interact (Mouse Click)"));

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

void AMain_UiManager::SetUiEnabled(UiType type, bool bEnabled)
{
	TObjectPtr<AActor> uiHandlerActor = nullptr;
	// Add switch statement here.

	if (!IsValid(uiHandlerActor)) return;

	if (IUiHandler* uiHandler = Cast<IUiHandler>(uiHandlerActor))
		uiHandler->SetUiEnabled(bEnabled);
}

void AMain_UiManager::SetSeedText(int32 seed)
{
	if (IsValid(coordinatesText))
	{
		FString coordText = FString::Printf(TEXT("Eigenvalue : %010d"), seed);
		coordinatesText->SetText(FText::FromString(coordText));
	}
}
