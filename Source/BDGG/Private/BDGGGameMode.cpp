// Copyright Epic Games, Inc. All Rights Reserved.

#include "BDGGGameMode.h"
#include "BDGGCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameModeWidget.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "EngineUtils.h"

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

}

AActor* ABDGGGameMode::ChoosePlayerStart_Implementation(AController* player)
{
	return Super::ChoosePlayerStart_Implementation(player);

	for (TActorIterator<APlayerStart> iter(GetWorld()); iter; ++iter)
	{
		APlayerStart* ps = *iter;
		if (ps->PlayerStartTag != FName("Spawned"))
		{
			ps->PlayerStartTag = FName("Spawned");
			return ps;
		}
	}
	return nullptr;
}


