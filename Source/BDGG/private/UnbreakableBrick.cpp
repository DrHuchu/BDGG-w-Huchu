// Fill out your copyright notice in the Description page of Project Settings.


#include "UnbreakableBrick.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AUnbreakableBrick::AUnbreakableBrick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(boxComp);
	meshComp->SetRelativeScale3D(FVector(0.25f));
}

// Called when the game starts or when spawned
void AUnbreakableBrick::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnbreakableBrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

