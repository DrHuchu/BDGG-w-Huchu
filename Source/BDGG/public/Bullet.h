// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class BDGG_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//변수 관련
	UPROPERTY(EditAnywhere, Category = "Bullet")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* lightningBullet;

	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	class ABrickBase* brick;

	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	class AUnbreakable* unbreakable;

	UPROPERTY(VisibleAnywhere, Category = "Bullet")
	class ABDGGPlayer* player;

	UPROPERTY(VIsibleAnywhere, Category = "Bullet")
	ABullet* bullet;

	//함수 관련
	UFUNCTION()
	virtual void BulletCrash(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult);


};
