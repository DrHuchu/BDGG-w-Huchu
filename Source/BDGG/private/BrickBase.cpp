// Fill out your copyright notice in the Description page of Project Settings.


#include "BrickBase.h"

#include "BDGGGameMode.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
ABrickBase::ABrickBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(boxComp);
	meshComp->SetRelativeScale3D(FVector(0.25f));

	destructibleMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("DestructibleMesh"));
	destructibleMesh->SetupAttachment(boxComp);
	destructibleMesh->bIgnoreRadialForce = true;
	destructibleMesh->bIgnoreRadialImpulse = true;
	destructibleMesh->DamageThreshold[0] = 0.1;
	destructibleMesh->DamageThreshold[1] = 0.1;
	destructibleMesh->DamageThreshold[2] = 0.1;

	

}

// Called when the game starts or when spawned
void ABrickBase::BeginPlay()
{
	Super::BeginPlay();

	destructibleMesh->SetVisibility(false);
	destructibleMesh->SetSimulatePhysics(false);
	destructibleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	gm = Cast<ABDGGGameMode>(GetWorld()->GetAuthGameMode());

	meshComp->OnComponentPhysicsStateChanged.AddDynamic(this, &ABrickBase::ParticleDownsize);
}

// Called every frame
void ABrickBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsBroken)
	{
		destructibleMesh->SetRelativeScale3D(destructibleMesh->GetRelativeScale3D() * 0.95f);
	}
}

void ABrickBase::OnBlockHit()
{
	//점수를 올린다.
	AddScore();
}

void ABrickBase::AddScore()
{
	UE_LOG(LogTemp, Warning, TEXT("Score ++*3"));
	if (gm)
	{
		//gamestate의 점수 1점 득점
		//gm->GameState->PlayerArray
	}


	meshComp->SetHiddenInGame(true);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AFieldSystemActor* field = GetWorld()->SpawnActor<class AFieldSystemActor>(masterField, destructibleMesh->GetComponentLocation(), destructibleMesh->GetComponentRotation());
	destructibleMesh->SetVisibility(true);
	destructibleMesh->SetSimulatePhysics(true);
	destructibleMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	destructibleMesh->SetCollisionProfileName(FName("Destructible"));

	destructibleMesh->AddForceAtLocation(hitDirection.GetSafeNormal() * 5000000.0f, GetActorLocation());


	//FTimerHandle masterFieldTimer;
	//GetWorldTimerManager().SetTimer(masterFieldTimer, FTimerDelegate::CreateLambda([&]()
	//	{
	//		if (field)
	//		{
	//			UE_LOG(LogTemp, Warning, TEXT("Field Destroy"));
	//		}
	//	}), 0.2f, false);

	//3초 후에 완전히 파괴
	FTimerHandle destroyTimer;
	GetWorldTimerManager().SetTimer(destroyTimer, FTimerDelegate::CreateLambda([&]()
		{
			UE_LOG(LogTemp, Warning, TEXT("Destroy"));
			Destroy();
		}), 3.0f, false);
}

void ABrickBase::ParticleDownsize(UPrimitiveComponent* ChangedComponent, EComponentPhysicsStateChange StateChange)
{
	FTimerHandle particleTimer;
	GetWorldTimerManager().SetTimer(particleTimer, FTimerDelegate::CreateLambda([&]()
		{
			bIsBroken = true;
		}), 2.0f, false);

}
