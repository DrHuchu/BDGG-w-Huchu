// Copyright Epic Games, Inc. All Rights Reserved.

#include "BDGGGameMode.h"
#include "BDGGCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextBlock.h"
#include "GameModeWidget.h"

ABDGGGameMode::ABDGGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UGameModeWidget> gameModeWidgetTemp(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LTG/Blueprints/BP_GameModeWidget.BP_GameModeWidget_C'"));
	if (gameModeWidgetTemp.Succeeded())
	{
		gameModeWidgetFactory = gameModeWidgetTemp.Class;
	}
}

void ABDGGGameMode::BeginPlay()
{
	Super::BeginPlay();

	gameModeWidgetUI = CreateWidget<UGameModeWidget>(GetWorld(), gameModeWidgetFactory);
	gameModeWidgetUI->AddToViewport();

	StartWidgetPlay();
}

void ABDGGGameMode::CountDownTimer(int TimeInSec)
{
	countDownTime = TimeInSec;
	UpdateMinAndSec();
	GetWorldTimerManager().SetTimer(countDownTimerHandle, FTimerDelegate::CreateLambda([&](){
		if (countDownTime > 0)
		{
			countDownTime--;
			UpdateMinAndSec();
		}
		else
		{
			countDownTime = 0;
			GetWorldTimerManager().ClearTimer(countDownTimerHandle);
		}
		}), 1.f, true);
}

void ABDGGGameMode::StartWidgetPlay()
{
	GetWorldTimerManager().SetTimer(startCountHandle, FTimerDelegate::CreateLambda([&](){
		if (startCountNum != 0)
		{
			gameModeWidgetUI->TextBlock_StartCount->SetText(FText::AsNumber(startCountNum));
		}
		else
		{
			gameModeWidgetUI->TextBlock_StartCount->SetText(FText::FromString("Start!"));
			GetWorldTimerManager().ClearTimer(startCountHandle);
			CountDownTimer(120);
		}
		gameModeWidgetUI->PlayAnimation(gameModeWidgetUI->Anim_StartCount);
		startCountNum--;
	}),1.f, true);
}

void ABDGGGameMode::UpdateMinAndSec()
{
	countDownTimeMin = countDownTime / 60;
	countDownTimeSec = countDownTime % 60;

	if (gameModeWidgetUI)
	{
		gameModeWidgetUI->TextBlock_Min->SetVisibility(ESlateVisibility::Visible);
		gameModeWidgetUI->TextBlock_Sec->SetVisibility(ESlateVisibility::Visible);
		gameModeWidgetUI->TextBlock_Dot->SetVisibility(ESlateVisibility::Visible);

		gameModeWidgetUI->TextBlock_Min->SetText(FText::AsNumber(countDownTimeMin));
		gameModeWidgetUI->TextBlock_Sec->SetText(FText::AsNumber(countDownTimeSec));
	}
}
