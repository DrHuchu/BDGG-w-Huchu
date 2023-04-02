// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BDGGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BDGG_API ABDGGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ABDGGPlayerController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGameModeWidget> gameModeWidgetFactory;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ULobbyWidget> lobbyWidgetFactory;

	UPROPERTY(BlueprintReadOnly)
	class UGameModeWidget* gameModeWidgetUI;
	UPROPERTY(BlueprintReadOnly)
	class ULobbyWidget* lobbyWidgetUI;

};
