// Fill out your copyright notice in the Description page of Project Settings.


#include "BDGGPlayerController.h"
#include "LobbyWidget.h"
#include "GameModeWidget.h"

ABDGGPlayerController::ABDGGPlayerController()
{
	static ConstructorHelpers::FClassFinder<UGameModeWidget> gameModeWidgetTemp(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LTG/UI/WBP_GameModeWidget.WBP_GameModeWidget_C'"));
	if (gameModeWidgetTemp.Succeeded())
	{
		gameModeWidgetFactory = gameModeWidgetTemp.Class;
		gameModeWidgetUI = CreateWidget<UGameModeWidget>(GetWorld(), gameModeWidgetFactory);
	}

	static ConstructorHelpers::FClassFinder<ULobbyWidget> lobbyWidgetTemp(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LTG/UI/WBP_LobbyWidget.WBP_LobbyWidget_C'"));
	if (lobbyWidgetTemp.Succeeded())
	{
		lobbyWidgetFactory = lobbyWidgetTemp.Class;
		lobbyWidgetUI = CreateWidget<ULobbyWidget>(GetWorld(), lobbyWidgetFactory);
	}
}

void ABDGGPlayerController::BeginPlay()
{
	
}

void ABDGGPlayerController::Tick(float DeltaSeconds)
{
	
}

