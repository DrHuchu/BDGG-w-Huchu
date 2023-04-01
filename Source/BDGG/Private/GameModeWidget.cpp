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
		//GetWorld()->GetGameState()->PlayerArray[0]->SetPlayerName(gi->sessionID.ToString());
		gi->GetFirstLocalPlayerController()->GetPawn()->GetPlayerState()->SetPlayerName(gi->sessionID.ToString());
	}
	// 시작 카운트다운
	StartWidgetPlay();
}

void UGameModeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	RefreshRank();
}

void UGameModeWidget::RefreshRanking()
{
	int arraySize = GetWorld()->GetGameState()->PlayerArray.Num();

	
	// 플레이어 스테이트 배열을 가져와서 임시변수에 복사한다
	auto playerStateArray = GetWorld()->GetGameState()->PlayerArray;

	/*
	// 임시변수에 복사한 배열을 점수에 따라 sort한다
	playerStateArray.Sort([](const TObjectPtr<APlayerState>& A, const TObjectPtr<APlayerState>& B)
		{
			return A->GetScore() < B->GetScore();
		});
	*/

	/*
	TArray<APlayerState*> playerStateArray;
	playerStateArray.Reserve(arraySize);

	for (APlayerState* Player : GetWorld()->GetGameState()->PlayerArray)
	{
			const int32 Score = Player->GetScore();
			int32 InsertIndex = arraySize - 1;

			while (InsertIndex > 0 && playerStateArray[InsertIndex]->GetScore() > Score)
			{
				InsertIndex--;
			}

			playerStateArray.Insert(Player, InsertIndex);
		}
	*/




	// sort한 배열의 순서대로 점수표에 출력한다

	switch (arraySize)
	{
	case 1:
		//임시점수
		//GetWorld()->GetGameState()->PlayerArray[0]->SetScore(2000);
		// id 출력
		TextBlock_RankID1->SetText(FText::FromString(playerStateArray[0]->GetPlayerName()));
		//TextBlock_RankID1->SetText(FText::FromString(GetGameInstance()->GetFirstLocalPlayerController()->GetPawn()->GetPlayerState()->GetPlayerName()));
		
		
		TextBlock_RankID2->SetVisibility(ESlateVisibility::Hidden);
		TextBlock_RankID3->SetVisibility(ESlateVisibility::Hidden);
		TextBlock_RankID4->SetVisibility(ESlateVisibility::Hidden);
		// score 출력
		if (tempScore1 < playerStateArray[0]->GetScore())
		{
			TextBlock_RankScore1->SetText(FText::AsNumber(tempScore1 += scoreSpeed));
		}
		TextBlock_RankScore2->SetVisibility(ESlateVisibility::Hidden);
		TextBlock_RankScore3->SetVisibility(ESlateVisibility::Hidden);
		TextBlock_RankScore4->SetVisibility(ESlateVisibility::Hidden);
		break;
	case 2:
		TextBlock_RankID1->SetText(FText::FromString(playerStateArray[0]->GetPlayerName()));
		TextBlock_RankID2->SetText(FText::FromString(playerStateArray[1]->GetPlayerName()));
		TextBlock_RankID2->SetVisibility(ESlateVisibility::Visible);
		TextBlock_RankID3->SetVisibility(ESlateVisibility::Hidden);
		TextBlock_RankID4->SetVisibility(ESlateVisibility::Hidden);

		if (tempScore1 < playerStateArray[0]->GetScore())
		{
			TextBlock_RankScore1->SetText(FText::AsNumber(tempScore1 += scoreSpeed));
		}
		if (tempScore2 < playerStateArray[1]->GetScore())
		{
			TextBlock_RankScore2->SetText(FText::AsNumber(tempScore2 += scoreSpeed));
		}
		TextBlock_RankScore2->SetVisibility(ESlateVisibility::Visible);
		TextBlock_RankScore3->SetVisibility(ESlateVisibility::Hidden);
		TextBlock_RankScore4->SetVisibility(ESlateVisibility::Hidden);
		break;
	case 3:
		TextBlock_RankID1->SetText(FText::FromString(GetWorld()->GetGameState()->PlayerArray[0]->GetPlayerName()));
		TextBlock_RankID2->SetText(FText::FromString(GetWorld()->GetGameState()->PlayerArray[1]->GetPlayerName()));
		TextBlock_RankID3->SetText(FText::FromString(GetWorld()->GetGameState()->PlayerArray[2]->GetPlayerName()));
		TextBlock_RankID3->SetVisibility(ESlateVisibility::Visible);
		TextBlock_RankID4->SetVisibility(ESlateVisibility::Hidden);

		TextBlock_RankScore1->SetText(FText::AsNumber(GetWorld()->GetGameState()->PlayerArray[0]->GetScore()));
		TextBlock_RankScore2->SetText(FText::AsNumber(GetWorld()->GetGameState()->PlayerArray[1]->GetScore()));
		TextBlock_RankScore3->SetText(FText::AsNumber(GetWorld()->GetGameState()->PlayerArray[2]->GetScore()));
		TextBlock_RankScore3->SetVisibility(ESlateVisibility::Visible);
		TextBlock_RankScore4->SetVisibility(ESlateVisibility::Hidden);
		break;
	case 4:
		TextBlock_RankID1->SetText(FText::FromString(GetWorld()->GetGameState()->PlayerArray[0]->GetPlayerName()));
		TextBlock_RankID2->SetText(FText::FromString(GetWorld()->GetGameState()->PlayerArray[1]->GetPlayerName()));
		TextBlock_RankID3->SetText(FText::FromString(GetWorld()->GetGameState()->PlayerArray[2]->GetPlayerName()));
		TextBlock_RankID4->SetText(FText::FromString(GetWorld()->GetGameState()->PlayerArray[3]->GetPlayerName()));
		TextBlock_RankID4->SetVisibility(ESlateVisibility::Visible);

		TextBlock_RankScore1->SetText(FText::AsNumber(GetWorld()->GetGameState()->PlayerArray[0]->GetScore()));
		TextBlock_RankScore2->SetText(FText::AsNumber(GetWorld()->GetGameState()->PlayerArray[1]->GetScore()));
		TextBlock_RankScore3->SetText(FText::AsNumber(GetWorld()->GetGameState()->PlayerArray[2]->GetScore()));
		TextBlock_RankScore4->SetText(FText::AsNumber(GetWorld()->GetGameState()->PlayerArray[3]->GetScore()));
		TextBlock_RankScore4->SetVisibility(ESlateVisibility::Visible);
		break;
	}
}

void UGameModeWidget::RefreshRank()
{
	int arraySize = GetWorld()->GetGameState()->PlayerArray.Num();
	auto playerStateArray = GetWorld()->GetGameState()->PlayerArray;

	TextBlock_RankID1->SetText(FText::FromString(playerStateArray[0]->GetPlayerName()));
	if (tempScore1 < playerStateArray[0]->GetScore())
	{
		TextBlock_RankScore1->SetText(FText::AsNumber(tempScore1 += scoreSpeed));
	}

	if (arraySize == 2)
	{
		TextBlock_RankID2->SetText(FText::FromString(playerStateArray[1]->GetPlayerName()));
		if (tempScore2 < playerStateArray[1]->GetScore())
		{
			TextBlock_RankScore2->SetText(FText::AsNumber(tempScore2 += scoreSpeed));
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
