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
};
