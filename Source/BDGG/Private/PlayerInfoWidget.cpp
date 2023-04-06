
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoWidget.h"
#include "Components/TextBlock.h"
#include "BDGGplayer.h"


void UPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}

void UPlayerInfoWidget::Setplayer(class ABDGGPlayer* player)
{

}
