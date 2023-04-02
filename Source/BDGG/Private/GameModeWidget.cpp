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
	// 게임모드 캐스팅
	gm = Cast<ABDGGGameMode>(GetWorld()->GetAuthGameMode());
	// 게임 인스턴스 캐스팅
	UBDGGGameInstance* gi = Cast<UBDGGGameInstance>(GetGameInstance());
	if (gi)
	{
		//gi->GetFirstLocalPlayerController()->GetPawn()->GetPlayerState()->SetPlayerName(gi->sessionID.ToString());
		GetOwningPlayerState()->SetPlayerName(gi->sessionID.ToString());
	}
	// 시작 카운트다운
	StartWidgetPlay();

	// 초기화에 쓰일 빈 구조체
	tempStruct.name = {};
	tempStruct.score = 0;
	// 랭킹표 구성요소들을 담은 배열
	textblockRankIdArray = { TextBlock_RankID1, TextBlock_RankID2, TextBlock_RankID3, TextBlock_RankID4 };
	textblockRankScoreArray = { TextBlock_RankScore1, TextBlock_RankScore2, TextBlock_RankScore3, TextBlock_RankScore4 };
	tempScoreArray = { tempScore1, tempScore2, tempScore3, tempScore4 };
}

void UGameModeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	// 틱마다 랭킹 갱신
	RefreshRanking();
}

void UGameModeWidget::RefreshRanking()
{
	// playerstate array 가져오기
	auto playerStateArray = GetWorld()->GetGameState()->PlayerArray;
	// arraysize 가져오기
	int arraySize = playerStateArray.Num();
	// 정보 담을 구조체 배열 만들기
	TArray<FPlayerInfo> nameScoreArr;
	// 배열 초기화
	nameScoreArr.Init(tempStruct, arraySize);
	// 구조체에 각각 이름과 점수 채워주기
	for(int i = 0; i < arraySize; i++)
	{
		nameScoreArr[i].name = playerStateArray[i]->GetPlayerName();
		nameScoreArr[i].score = playerStateArray[i]->GetScore();
	}
	// 구조체 배열을 점수에 따라 정렬
	nameScoreArr.Sort([](const FPlayerInfo& A, const FPlayerInfo& B)
		{
			return A.score > B.score;
		});
	// 정렬된 순서대로 랭킹표에 출력
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

			// 엔딩
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
