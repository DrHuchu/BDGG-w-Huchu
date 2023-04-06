// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeWidget.h"

#include "BDGGPlayerController.h"
#include "BDGGPlayerState.h"
#include "BDGGGameState.h"
#include "BDGGPlayer.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "EngineUtils.h"

void UGameModeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// 컨트롤러 캐싱
	pc = Cast<ABDGGPlayerController>(GetOwningLocalPlayer()->GetPlayerController(GetWorld()));

	// 랭킹표 구성요소들을 담은 배열
	textblockRankIdArray = { TextBlock_RankID1, TextBlock_RankID2, TextBlock_RankID3, TextBlock_RankID4 };
	textblockRankScoreArray = { TextBlock_RankScore1, TextBlock_RankScore2, TextBlock_RankScore3, TextBlock_RankScore4 };
	tempScoreArray = { tempScore1, tempScore2, tempScore3, tempScore4 };

	gs = Cast<ABDGGGameState>(GetWorld()->GetGameState());

	// 시작전까지 못 움직이게 세팅
	if (GetOwningPlayerPawn())
	{
		GetOwningPlayerPawn()->DisableInput(pc);
	}

	// 게임 끝나고 나가기 버튼 바인딩
	btn_Quit->OnClicked.AddDynamic(this, &UGameModeWidget::QuitGame);

	// 게임끝나고 재접속 시 doOnce변수들 초기화
	if (GetWorld()->GetMapName().Contains("Lobby"))
	{
		bDoOnce = false;
		bIsDesolved = false;
	}
}

void UGameModeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	RefreshRanking();
	//UE_LOG(LogTemp, Warning, TEXT("state : %s"), *gs->GetMatchState().ToString());

	if (gs->GetMatchState() == FName("Started") && !bDoOnce)
	{
		bDoOnce = true;
		UE_LOG(LogTemp, Warning, TEXT("do once in"));
		if (GetOwningPlayerPawn()->HasAuthority())
		{
			FTimerHandle hd;
			GetWorld()->GetTimerManager().SetTimer(hd, FTimerDelegate::CreateLambda([&]()
				{
					StartWidgetPlay();
				}), 1.f, false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("start widget play"));
			StartWidgetPlay();
		}
	}
}

void UGameModeWidget::RefreshRanking()
{
	// playerstate array 가져오기
	auto playerStateArray = GetWorld()->GetGameState()->PlayerArray;

	playerStateArray.Sort([](const APlayerState& A, const APlayerState& B)
		{
			return A.GetScore() > B.GetScore();
		});

	// 정렬된 순서대로 랭킹표에 출력
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
			CountDownTimer(playTime);
			GetWorld()->GetTimerManager().ClearTimer(startCountHandle);
			AllPlayerCanMoveServer();
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

	if (countDownTime <= 15 && !bIsDesolved)
	{
		bIsDesolved = true;
		PlayAnimation(Anim_DesolveScoreChart);
	}
}

void UGameModeWidget::GameEnd()
{
	AllPlayerDontMoveServer();

	// 승자처리
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

	GetOwningPlayerState()->GetPlayerController()->SetShowMouseCursor(true);
}

void UGameModeWidget::ResetScoreBeforeGameEnd()
{
	tempScore1 = 0;
	tempScore2 = 0;
	tempScore3 = 0;
	tempScore4 = 0;
}

void UGameModeWidget::QuitGame()
{
	if (GetOwningPlayer()->HasAuthority())
	{
		for (TActorIterator<ABDGGPlayer> pl(GetWorld()); pl; ++pl)
		{
			ABDGGPlayer* player = *pl;
			if (!player->HasAuthority())
			{
				auto controller = Cast<ABDGGPlayerController>(player->GetController());
				if (controller && controller->IsLocalController())
				{
					controller->ServerEndSession();
				}
			}
		}

		FTimerHandle destroySessionHandle;
		GetWorld()->GetTimerManager().SetTimer(destroySessionHandle, FTimerDelegate::CreateLambda([&]()
		{
			pc->ServerEndSession();
		}), 1.f, false);
	}
	else
	{
		pc->ServerEndSession();
	}

}

void UGameModeWidget::AllPlayerDontMoveServer_Implementation()
{
	AllPlayerDontMoveMulti();
}

void UGameModeWidget::AllPlayerCanMoveServer_Implementation()
{
	AllPlayerCanMoveMulti();
}

void UGameModeWidget::AllPlayerDontMoveMulti_Implementation()
{
	GetOwningPlayerPawn()->DisableInput(pc);
}

void UGameModeWidget::AllPlayerCanMoveMulti_Implementation()
{
	GetOwningPlayerPawn()->EnableInput(pc);
}