// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/TurnOrderFrameWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "UI/Widget/CharacterTurnHpWidget.h"
#include "Components/HorizontalBoxSlot.h"

void UTurnOrderFrameWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UTurnOrderFrameWidget::CreateFrameWidget()
{
	PortraitFrame->ClearChildren();
	
	UHorizontalBoxSlot* decoL = PortraitFrame->AddChildToHorizontalBox(SideDeco_L);
	decoL->SetPadding(FMargin(0.f, 0.f, 5.f, 0.f));
	decoL->SetHorizontalAlignment(HAlign_Left);
	decoL->SetVerticalAlignment(VAlign_Top);
	
	for (auto* widget : WidgetToAddArray)
	{
		UHorizontalBoxSlot* hbSlot = PortraitFrame->AddChildToHorizontalBox(widget);
	}

	UHorizontalBoxSlot* decoR = PortraitFrame->AddChildToHorizontalBox(SideDeco_R);
	decoR->SetPadding(FMargin(5.f, 0.f, 0.f, 0.f));
	decoR->SetHorizontalAlignment(HAlign_Right);
	decoR->SetVerticalAlignment(VAlign_Top);
}
