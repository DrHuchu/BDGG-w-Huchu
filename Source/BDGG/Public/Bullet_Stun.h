// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "Bullet_Stun.generated.h"

/**
 * 
 */
UCLASS()
class BDGG_API ABullet_Stun : public ABullet
{
	GENERATED_BODY()

public:
	virtual void BulletCrash(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY()
	class APlayerController* pc;

	UFUNCTION()
	void StunOver();

	UPROPERTY(EditAnywhere)
	class UParticleSystem* lightning;

	UFUNCTION(NetMulticast, Unreliable)
	void Stunned();

};
