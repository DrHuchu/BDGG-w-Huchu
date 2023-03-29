// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrickBase.h"
#include "Brick_3rd.generated.h"

/**
 * 
 */
UCLASS()
class BDGG_API ABrick_3rd : public ABrickBase
{
	GENERATED_BODY()

	virtual void AddScore() override;

public:
	UPROPERTY(EditAnywhere, Category = "Brick")
		int32 brickScore3 = 3000;

	UPROPERTY(VisibleAnywhere, Category = "Brick")
		int32 brickMaxHP = 20;

	UPROPERTY(VisibleAnywhere, Category = "Brick")
		int32 brickHP = brickMaxHP;

	UPROPERTY(VisibleAnywhere, Category = "Brick")
		float mixValue = 0;

	UPROPERTY(VisibleAnywhere, Category = "Brick")
		float power = 30;
};
