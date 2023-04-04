// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeWidget.h"
#include "BDGGGameMode.generated.h"

UCLASS(minimalapi)
class ABDGGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABDGGGameMode();
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* player) override;
};



