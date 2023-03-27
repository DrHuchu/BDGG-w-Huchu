// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionSlotWidget.h"
#include "BDGGGameInstance.h"
#include "Components/Button.h"

void USessionSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	gameInstance = Cast<UBDGGGameInstance>(GetGameInstance());

	btn_roomName->OnClicked.AddDynamic(this, &USessionSlotWidget::JoinRoom);
}

void USessionSlotWidget::JoinRoom()
{
	if (gameInstance)
	{
		gameInstance->JoinMySession(slotIdx);
	}
}
