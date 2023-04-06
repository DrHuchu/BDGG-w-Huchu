// Copyright Epic Games, Inc. All Rights Reserved.

#include "BDGGGameMode.h"
#include "BDGGGameInstance.h"
#include "BDGGGameState.h"
#include "BDGGPlayer.h"
#include "BDGGPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "GameModeWidget.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "GameFramework/GameStateBase.h"

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

	connectedPlayerNum++;

	//UE_LOG(LogTemp, Warning, TEXT("MapName is %s"), *GetWorld()->GetMapName());

	gi = Cast<UBDGGGameInstance>(GetGameInstance());
	if (gi && GetWorld()->GetMapName().Contains("Huchu"))
	{
		FTimerHandle delayHandle;
		GetWorldTimerManager().SetTimer(delayHandle, FTimerDelegate::CreateLambda([&]()
			{
				totalPlayerNum = gi->totalPlayerNum;
				//UE_LOG(LogTemp, Warning, TEXT("Current %d / Total %d"), connectedPlayerNum, totalPlayerNum);
				if (connectedPlayerNum == totalPlayerNum && totalPlayerNum != 0)
				{
					GetGameState<ABDGGGameState>()->SetMatchState(FName("Started"));
					//UE_LOG(LogTemp, Warning, TEXT("State Seted!!!!!!!!!!!!"));
				}
			}), 1.f, false);
	}
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


