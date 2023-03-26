// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeWidget.h"
#include "BDGGGameMode.generated.h"

UCLASS(minimalapi)
class ABDGGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABDGGGameMode();
	virtual void BeginPlay() override;

	int countDownTime;
	int countDownTimeMin;
	int countDownTimeSec;
	int startCountNum = 3;

	FTimerHandle countDownTimerHandle;
	FTimerHandle startCountHandle;

	void CountDownTimer(int TimeInSec);
	void StartWidgetPlay();
	void UpdateMinAndSec();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGameModeWidget> gameModeWidgetFactory;
	UPROPERTY()
	class UGameModeWidget* gameModeWidgetUI;
};



