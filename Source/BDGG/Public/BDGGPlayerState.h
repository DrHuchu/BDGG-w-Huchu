// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BDGGPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BDGG_API ABDGGPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
