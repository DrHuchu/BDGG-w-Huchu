// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameModeWidget.generated.h"

UCLASS()
class BDGG_API UGameModeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY()
	class ABDGGPlayerController* pc;
	UPROPERTY()
	class ABDGGGameState* gs;

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
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_Quit;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Anim_StartCount;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Anim_EndScoreChart;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Anim_EndText;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Anim_DesolveScoreChart;

	void RefreshRanking();

	// 타이머 관련
	int countDownTime;
	int countDownTimeMin;
	int countDownTimeSec;
	int startCountNum = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int playTime = 60;

	FTimerHandle countDownTimerHandle;
	FTimerHandle startCountHandle;

	UFUNCTION()
	void CountDownTimer(int TimeInSec);
	UFUNCTION()
	void StartWidgetPlay();
	UFUNCTION()
	void UpdateMinAndSec();
	UFUNCTION()
	void GameEnd();
	UFUNCTION(Server, Unreliable)
	void ResetScoreBeforeGameEnd();
	UFUNCTION(NetMulticast, Unreliable)
	void ResetScoreBeforeGameEndMulti();

	int tempScore1;
	int tempScore2;
	int tempScore3;
	int tempScore4;

	bool bDoOnce;
	bool bIsDesolved;

	UPROPERTY(EditDefaultsOnly)
	int scoreSpeed = 10;

	UPROPERTY()
	TArray <UTextBlock*> textblockRankIdArray;
	UPROPERTY()
	TArray <UTextBlock*> textblockRankScoreArray;
	TArray <int> tempScoreArray;

	FString myID;
	FString winnerID;

	UFUNCTION(Server, Unreliable)
	void AllPlayerDontMoveServer();
	UFUNCTION(NetMulticast, Unreliable)
	void AllPlayerDontMoveMulti();
	UFUNCTION(Server, Unreliable)
	void AllPlayerCanMoveServer();
	UFUNCTION(NetMulticast, Unreliable)
	void AllPlayerCanMoveMulti();
	UFUNCTION()
	void QuitGame();

};
