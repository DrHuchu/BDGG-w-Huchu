// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/StaticMeshComponent.h"
#include "BDGGPlayer.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	SetRootComponent(meshComp);
	meshComp->SetCollisionProfileName(FName("Item"));
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	meshComp->OnComponentBeginOverlap.AddDynamic(this, &AItem::ItemGet);
	meshComp->SetSimulatePhysics(true);
	
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::ItemGet(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	player = Cast<ABDGGPlayer>(OtherActor);

	if(player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Stun gun bullet +1"));
		//플레이어의 총을 스턴 건으로 변경

		//플레이어가 갖고 있는 스턴건 총알 수 +1

		Destroy();
	}
}
