// Fill out your copyright notice in the Description page of Project Settings.


#include "BrickBase.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
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

	DestructibleMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("DestructedMesh"));
	DestructibleMesh->SetupAttachment(boxComp);
}

// Called when the game starts or when spawned
void ABrickBase::BeginPlay()
{
	Super::BeginPlay();
	DestructibleMesh->SetVisibility(false);
	DestructibleMesh->SetSimulatePhysics(false);
	DestructibleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABrickBase::OnBrickHit);
}

// Called every frame
void ABrickBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABrickBase::OnBrickHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	
}

