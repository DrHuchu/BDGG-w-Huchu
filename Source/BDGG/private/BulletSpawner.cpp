// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletSpawner.h"

#include "Bullet.h"
#include "Components/BoxComponent.h"

// Sets default values
ABulletSpawner::ABulletSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABulletSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABulletSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentTime += DeltaTime;

	if(currentTime > 1.5f)
	{
		GetWorld()->SpawnActor<ABullet>(bulletFactory, GetActorLocation() + GetActorForwardVector() * 20, GetActorRotation());
		currentTime = 0;
	}
}

