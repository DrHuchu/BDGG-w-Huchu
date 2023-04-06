// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrickBase.h"
#include "ItemBrick.generated.h"

/**
 * 
 */
UCLASS()
class BDGG_API AItemBrick : public ABrickBase
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, Category = "Brick")
		int32 brickMaxHP = 2;

	UPROPERTY(VisibleAnywhere, Category = "Brick")
		int32 brickHP = brickMaxHP;

	UPROPERTY(VisibleAnywhere, Category = "Brick")
		float mixValue = 0;

	UPROPERTY(VisibleAnywhere, Category = "Brick")
		float power = 30;

	UPROPERTY(EditAnywhere, Category = "Brick")
		TSubclassOf<class AItem> itemFactory;

	virtual void AddScore_Implementation() override;

	virtual void SpawnFX_Implementation() override;;
};
