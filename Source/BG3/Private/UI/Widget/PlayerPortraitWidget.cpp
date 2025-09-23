// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerPortraitWidget.h"


UPlayerPortraitWidget::UPlayerPortraitWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UMaterialInterface> portraitMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Assets/UI/BottomBar/Materials/M_Portrait_link_Inst.M_Portrait_link_Inst'"));
	if (portraitMaterial.Succeeded())
	{
		PortraitMaterial = portraitMaterial.Object;
	}
}

void UPlayerPortraitWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	PortraitMID = UMaterialInstanceDynamic::Create(PortraitMaterial, this);
}

void UPlayerPortraitWidget::ChangePortrait(UTexture2D* texture)
{
	PortraitMID->SetTextureParameterValue(FName(TEXT("InputTexture")), texture);
}
