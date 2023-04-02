// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeWidget.h"

#include "BDGGGameInstance.h"
#include "BDGGGameMode.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void UGameModeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// ���Ӹ�� ĳ����
	gm = Cast<ABDGGGameMode>(GetWorld()->GetAuthGameMode());
	// ���� �ν��Ͻ� ĳ����
	UBDGGGameInstance* gi = Cast<UBDGGGameInstance>(GetGameInstance());
	if (gi)
	{
		//gi->GetFirstLocalPlayerController()->GetPawn()->GetPlayerState()->SetPlayerName(gi->sessionID.ToString());
		GetOwningPlayerState()->SetPlayerName(gi->sessionID.ToString());
	}
	// ���� ī��Ʈ�ٿ�
	StartWidgetPlay();

	// �ʱ�ȭ�� ���� �� ����ü
	tempStruct.name = {};
	tempStruct.score = 0;
	// ��ŷǥ ������ҵ��� ���� �迭
	textblockRankIdArray = { TextBlock_RankID1, TextBlock_RankID2, TextBlock_RankID3, TextBlock_RankID4 };
	textblockRankScoreArray = { TextBlock_RankScore1, TextBlock_RankScore2, TextBlock_RankScore3, TextBlock_RankScore4 };
	tempScoreArray = { tempScore1, tempScore2, tempScore3, tempScore4 };
}

void UGameModeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	// ƽ���� ��ŷ ����
	RefreshRanking();
}

void UGameModeWidget::RefreshRanking()
{
	// playerstate array ��������
	auto playerStateArray = GetWorld()->GetGameState()->PlayerArray;
	// arraysize ��������
	int arraySize = playerStateArray.Num();
	// ���� ���� ����ü �迭 �����
	TArray<FPlayerInfo> nameScoreArr;
	// �迭 �ʱ�ȭ
	nameScoreArr.Init(tempStruct, arraySize);
	// ����ü�� ���� �̸��� ���� ä���ֱ�
	for(int i = 0; i < arraySize; i++)
	{
		nameScoreArr[i].name = playerStateArray[i]->GetPlayerName();
		nameScoreArr[i].score = playerStateArray[i]->GetScore();
	}
	// ����ü �迭�� ������ ���� ����
	nameScoreArr.Sort([](const FPlayerInfo& A, const FPlayerInfo& B)
		{
			return A.score > B.score;
		});
	// ���ĵ� ������� ��ŷǥ�� ���
	for(int i = 0; i < arraySize; i++)
	{
		textblockRankIdArray[i]->SetText(FText::FromString(nameScoreArr[i].name));
		if (tempScoreArray[i] < nameScoreArr[i].score)
		{
			textblockRankScoreArray[i]->SetText(FText::AsNumber(tempScoreArray[i] += scoreSpeed));
		}
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

void UGameModeWidget::GameEnd()
{
	scoreSpeed = 50;
	ResetScoreBeforeGameEnd();
	PlayAnimation(Anim_EndScoreChart);
	TextBlock_StartCount->SetText(FText::FromString("Win!"));
	PlayAnimation(Anim_EndText);
}

void UGameModeWidget::ResetScoreBeforeGameEnd()
{
	tempScore1 = 0;
	tempScore2 = 0;
	tempScore3 = 0;
	tempScore4 = 0;
}
