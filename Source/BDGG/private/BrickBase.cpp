// Fill out your copyright notice in the Description page of Project Settings.


#include "BrickBase.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
ABrickBase::ABrickBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(boxComp);
	meshComp->SetRelativeScale3D(FVector(0.25f));

	destructibleMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("DestructibleMesh"));
	destructibleMesh->SetupAttachment(boxComp);

}

// Called when the game starts or when spawned
void ABrickBase::BeginPlay()
{
	Super::BeginPlay();

	destructibleMesh->SetVisibility(false);
	destructibleMesh->SetSimulatePhysics(false);
	destructibleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
}

// Called every frame
void ABrickBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABrickBase::OnBlockHit()
{
	//점수를 올린다.
	AddScore();
}

void ABrickBase::AddScore()
{
	UE_LOG(LogTemp, Warning, TEXT("Score ++"));
}
