// Copyright Epic Games, Inc. All Rights Reserved.

#include "BDGGGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextBlock.h"
#include "GameModeWidget.h"
#include "BDGGGameInstance.h"
#include "GameFramework/PlayerState.h"

ABDGGGameMode::ABDGGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ABDGGGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void ABDGGGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	//auto gi = Cast<UBDGGGameInstance>(NewPlayer->GetGameInstance());
	//NewPlayer->PlayerState->SetPlayerName(gi->sessionID.ToString());
}
