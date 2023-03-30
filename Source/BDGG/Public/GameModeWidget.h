// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameModeWidget.generated.h"

/**
 * 
 */
UCLASS()
class BDGG_API UGameModeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY()
	class ABDGGGameMode* gm;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TextBlock_Min;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TextBlock_Sec;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TextBlock_Dot;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TextBlock_StartCount;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TextBlock_RankID1;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TextBlock_RankID2;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TextBlock_RankID3;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TextBlock_RankID4;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TextBlock_RankScore1;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TextBlock_RankScore2;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TextBlock_RankScore3;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TextBlock_RankScore4;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UVerticalBox* VerticalBox_Ranking;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Anim_StartCount;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Anim_EndScoreChart;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Anim_EndText;

	void RefreshRanking();

	// 타이머 관련
	int countDownTime;
	int countDownTimeMin;
	int countDownTimeSec;
	int startCountNum = 3;

	UPROPERTY(EditDefaultsOnly)
	int playTime = 120;

	FTimerHandle countDownTimerHandle;
	FTimerHandle startCountHandle;

	void CountDownTimer(int TimeInSec);
	void StartWidgetPlay();
	void UpdateMinAndSec();
	void GameEnd();
	void ResetScoreBeforeGameEnd();

	int tempScore1;
	int tempScore2;
	int tempScore3;
	int tempScore4;

	UPROPERTY(EditDefaultsOnly)
	int scoreSpeed = 10;
};
