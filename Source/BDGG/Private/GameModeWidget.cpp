// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeWidget.h"

#include "BDGGGameMode.h"
#include "BDGGPlayerState.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void UGameModeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ���Ӹ�� ĳ����
	gm = Cast<ABDGGGameMode>(GetWorld()->GetAuthGameMode());

	// ���� ī��Ʈ�ٿ�
	StartWidgetPlay();

	// ��ŷǥ ������ҵ��� ���� �迭
	textblockRankIdArray = { TextBlock_RankID1, TextBlock_RankID2, TextBlock_RankID3, TextBlock_RankID4 };
	textblockRankScoreArray = { TextBlock_RankScore1, TextBlock_RankScore2, TextBlock_RankScore3, TextBlock_RankScore4 };
	tempScoreArray = { tempScore1, tempScore2, tempScore3, tempScore4 };
}

void UGameModeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	RefreshRanking();
}

void UGameModeWidget::RefreshRanking()
{
	// playerstate array ��������
	auto playerStateArray = GetWorld()->GetGameState()->PlayerArray;

	playerStateArray.Sort([](const APlayerState& A, const APlayerState& B)
		{
			return A.GetScore() > B.GetScore();
		});

	// ���ĵ� ������� ��ŷǥ�� ���
	for(int i = 0; i < playerStateArray.Num(); i++)
	{
		textblockRankIdArray[i]->SetVisibility(ESlateVisibility::Visible);
		textblockRankIdArray[i]->SetText(FText::FromString(playerStateArray[i]->GetPlayerName()));

		textblockRankScoreArray[i]->SetVisibility(ESlateVisibility::Visible);
		if (tempScoreArray[i] < playerStateArray[i]->GetScore())
		{
			textblockRankScoreArray[i]->SetText(FText::AsNumber(tempScoreArray[i] += scoreSpeed));
		}
	}
	if (!playerStateArray.IsEmpty())
	{
		winnerID = playerStateArray[0]->GetPlayerName();
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

			// ����
			GameEnd();
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
			CountDownTimer(playTime);
			GetWorld()->GetTimerManager().ClearTimer(startCountHandle);
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

void UGameModeWidget::GameEnd()
{
	// ����ó��
	auto ps = Cast<ABDGGPlayerState>(GetOwningPlayerState());
	if (ps)
	{
		myID = ps->GetPlayerName();
	}

	if (myID == winnerID)
	{
		TextBlock_StartCount->SetText(FText::FromString("Win!"));
	}
	else
	{
		TextBlock_StartCount->SetText(FText::FromString("Lose.."));
	}

	scoreSpeed = 50;
	ResetScoreBeforeGameEnd();
	PlayAnimation(Anim_EndScoreChart);
	PlayAnimation(Anim_EndText);
}

void UGameModeWidget::ResetScoreBeforeGameEnd()
{
	tempScore1 = 0;
	tempScore2 = 0;
	tempScore3 = 0;
	tempScore4 = 0;
}
