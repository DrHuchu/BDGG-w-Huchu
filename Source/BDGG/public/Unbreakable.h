// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrickBase.h"
#include "Unbreakable.generated.h"

/**
 * 
 */
UCLASS()
class BDGG_API AUnbreakable : public ABrickBase
{
	GENERATED_BODY()
	virtual void AddScore() override;
	virtual void BeginPlay() override;
};
