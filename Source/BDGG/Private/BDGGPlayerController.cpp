// Fill out your copyright notice in the Description page of Project Settings.


#include "BDGGPlayerController.h"

#include "BDGGGameInstance.h"
#include "BDGGPlayerState.h"
#include "LobbyWidget.h"
#include "GameModeWidget.h"
#include "GameFramework/PlayerState.h"

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
	gi = Cast<UBDGGGameInstance>(GetGameInstance());
	if (gi)
	{
		ServerSetName(gi->sessionID.ToString());
	}
}

void ABDGGPlayerController::MultiEndSession_Implementation()
{
	gi->sessionInterface->DestroySession(gi->sessionID);
	ClientTravel("/Game/Maps/LoginMap1", ETravelType::TRAVEL_Absolute);
}

void ABDGGPlayerController::ServerEndSession_Implementation()
{
	MultiEndSession();
}

void ABDGGPlayerController::ServerSetName_Implementation(const FString& name)
{
	GetPlayerState<ABDGGPlayerState>()->SetPlayerName(name);
}

