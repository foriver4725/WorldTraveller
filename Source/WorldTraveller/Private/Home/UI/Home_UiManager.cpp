#include "Home/UI/Home_UiManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "UiHandler.h"
#include "Home/UI/Home_StartGameUiHandler.h"
#include "UiZOrders.h"
#include "Enums/UiType.h"
#include "Enums/UiDescriptionTextType.h"

using DescTextType = EUiDescriptionTextType;
using UiType = EUiType;

AHome_UiManager::AHome_UiManager()
{
	currentDescriptionText = DescTextType::None;
}

void AHome_UiManager::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(widgetClass))
	{
		userWidget = CreateWidget<UUserWidget>(GetWorld(), widgetClass);
		if (IsValid(userWidget))
		{
			versionLabel = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("VersionLabel")));
			pointer = Cast<UImage>(userWidget->GetWidgetFromName(TEXT("Pointer")));
			descriptionText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("DescriptionText")));
			starAmountPanel = Cast<UCanvasPanel>(userWidget->GetWidgetFromName(TEXT("StarAmount")));
			starAmountText = Cast<UTextBlock>(userWidget->GetWidgetFromName(TEXT("StarAmountText")));

			if (UTextBlock* text = GetValid(versionLabel))
			{
				FString version;
				GConfig->GetString(
					TEXT("/Script/EngineSettings.GeneralProjectSettings"),
					TEXT("ProjectVersion"),
					version,
					GGameIni
				);

				text->SetText(FText::FromString(FString::Printf(TEXT("v %s"), *version)));
			}

			this->SetStarAmountUiEnabled(false);
			this->SetStarAmountText(0);

			userWidget->AddToViewport(FUiZOrders::Home_General);
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
		pointer->SetDesiredSizeOverride(bActivate ? FVector2D(16) : FVector2D(8));
	}
}

void AHome_UiManager::SetDescriptionText(DescTextType textType)
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

void AHome_UiManager::SetUiEnabled(UiType type, bool bEnabled)
{
	TObjectPtr<AActor> uiHandlerActor = nullptr;
	switch (type)
	{
	case UiType::Home_StartGame:
		uiHandlerActor = startGameUiHandler;
		break;
	default:
		break;
	}

	if (!IsValid(uiHandlerActor)) return;

	if (IUiHandler* uiHandler = Cast<IUiHandler>(uiHandlerActor))
		uiHandler->SetUiEnabled(bEnabled);
}

void AHome_UiManager::SetStarAmountUiEnabled(bool bEnabled)
{
	if (UWidget* p = GetValid(Cast<UWidget>(starAmountPanel)))
	{
		p->SetIsEnabled(bEnabled);
		p->SetVisibility(bEnabled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void AHome_UiManager::SetStarAmountText(uint64 starAmount)
{
	if (UTextBlock* p = GetValid(starAmountText))
		p->SetText(FText::FromString(FString::Printf(TEXT(" x %llu"), starAmount)));
}
