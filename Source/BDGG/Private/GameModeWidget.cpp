// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeWidget.h"

#include "BDGGGameMode.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void UGameModeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	gm = Cast<ABDGGGameMode>(GetWorld()->GetAuthGameMode());
	//StartWidgetPlay();
}

void UGameModeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	RefreshRanking();
}

void UGameModeWidget::RefreshRanking()
{
	//gm->GameState->PlayerArray
	// array¸¦ sort ÇÑ´Ù

	int arraySize = GetWorld()->GetGameState()->PlayerArray.Num();

	switch (arraySize)
	{
		case 1:
			TextBlock_RankID1->SetText(FText::FromString(GetWorld()->GetGameState()->PlayerArray[0]->GetPlayerName()));
			break;
		case 2:
			TextBlock_RankID1->SetText(FText::FromString(GetWorld()->GetGameState()->PlayerArray[0]->GetPlayerName()));
			TextBlock_RankID2->SetText(FText::FromString(GetWorld()->GetGameState()->PlayerArray[1]->GetPlayerName()));
			break;
		case 3:
			TextBlock_RankID1->SetText(FText::FromString(GetWorld()->GetGameState()->PlayerArray[0]->GetPlayerName()));
			TextBlock_RankID2->SetText(FText::FromString(GetWorld()->GetGameState()->PlayerArray[1]->GetPlayerName()));
			TextBlock_RankID3->SetText(FText::FromString(GetWorld()->GetGameState()->PlayerArray[2]->GetPlayerName()));
			break;
		case 4:
			TextBlock_RankID1->SetText(FText::FromString(GetWorld()->GetGameState()->PlayerArray[0]->GetPlayerName()));
			TextBlock_RankID2->SetText(FText::FromString(GetWorld()->GetGameState()->PlayerArray[1]->GetPlayerName()));
			TextBlock_RankID3->SetText(FText::FromString(GetWorld()->GetGameState()->PlayerArray[2]->GetPlayerName()));
			TextBlock_RankID4->SetText(FText::FromString(GetWorld()->GetGameState()->PlayerArray[3]->GetPlayerName()));
			break;
	}
}

void UGameModeWidget::CountDownTimer(int TimeInSec)
{
	countDownTime = TimeInSec;
	UpdateMinAndSec();
	GetWorld()->GetTimerManager().SetTimer(countDownTimerHandle, FTimerDelegate::CreateLambda([&]() {
		if (countDownTime > 0)
		{
			countDownTime--;
			UpdateMinAndSec();
		}
		else
		{
			countDownTime = 0;
			GetWorld()->GetTimerManager().ClearTimer(countDownTimerHandle);
		}
		}), 1.f, true);
}

void UGameModeWidget::StartWidgetPlay()
{
	GetWorld()->GetTimerManager().SetTimer(startCountHandle, FTimerDelegate::CreateLambda([&]() {
		if (startCountNum != 0)
		{
			TextBlock_StartCount->SetText(FText::AsNumber(startCountNum));
		}
		else
		{
			TextBlock_StartCount->SetText(FText::FromString("Start!"));
			GetWorld()->GetTimerManager().ClearTimer(startCountHandle);
			CountDownTimer(playTime);
		}
		PlayAnimation(Anim_StartCount);
		startCountNum--;
		}), 1.f, true);
}

void UGameModeWidget::UpdateMinAndSec()
{
	countDownTimeMin = countDownTime / 60;
	countDownTimeSec = countDownTime % 60;

	TextBlock_Min->SetVisibility(ESlateVisibility::Visible);
	TextBlock_Sec->SetVisibility(ESlateVisibility::Visible);
	TextBlock_Dot->SetVisibility(ESlateVisibility::Visible);

	TextBlock_Min->SetText(FText::AsNumber(countDownTimeMin));

	if (countDownTimeSec <= 9)
	{
		TextBlock_Sec->SetText(FText::FromString("0" + FString::FormatAsNumber(countDownTimeSec)));
	}
	else
	{
		TextBlock_Sec->SetText(FText::AsNumber(countDownTimeSec));
	}
}