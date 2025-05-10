#include "Home/UiManager_Home.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "EDescriptionText.h"

AUiManager_Home::AUiManager_Home() : Super()
{
}

void AUiManager_Home::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(widgetClass))
	{
		UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), widgetClass);
		if (widget)
		{
			pointer = Cast<UImage>(widget->GetWidgetFromName(TEXT("Pointer")));
			descriptionText = Cast<UTextBlock>(widget->GetWidgetFromName(TEXT("DescriptionText")));

			widget->AddToViewport();
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
