// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BrickBase.h"
#include "Brick_2nd.generated.h"

/**
 * 
 */
UCLASS()
class BDGG_API ABrick_2nd : public ABrickBase
{
	GENERATED_BODY()

	virtual void AddScore_Implementation() override;

public:
	UPROPERTY(EditAnywhere, Category = "Brick")
		int32 brickScore2 = 700;

	UPROPERTY(VisibleAnywhere, Category = "Brick")
		int32 brickMaxHP = 5;

	UPROPERTY(VisibleAnywhere, Category = "Brick")
		int32 brickHP = brickMaxHP;

	UPROPERTY(VisibleAnywhere, Category = "Brick")
		float mixValue = 0;

	UPROPERTY(VisibleAnywhere, Category = "Brick")
		float power = 30;

	UFUNCTION(NetMulticast, Unreliable)
	void ChangeColor();
};
