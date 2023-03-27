// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BDGGGameInstance.h"
#include "SessionSlotWidget.generated.h"

UCLASS()
class BDGG_API USessionSlotWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:

	UPROPERTY(meta = (BindWidget))
		class UButton* btn_roomName;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* text_roomName;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* text_playerInfo;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* text_ping;

	UPROPERTY()
		UBDGGGameInstance* gameInstance;

	int32 slotIdx;

private:

	UFUNCTION()
		void JoinRoom();
};
