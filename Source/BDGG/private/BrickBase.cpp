// Fill out your copyright notice in the Description page of Project Settings.


#include "BrickBase.h"

#include "BDGGGameMode.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"


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
	
}

// Called when the game starts or when spawned
void ABrickBase::BeginPlay()
{
	Super::BeginPlay();

	gm = Cast<ABDGGGameMode>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void ABrickBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABrickBase::OnBlockHit()
{
	//������ �ø���.
	AddScore();
}

void ABrickBase::AddScore()
{
	UE_LOG(LogTemp, Warning, TEXT("Score ++*3"));
	if (gm)
	{
		//���� ����
		UGameplayStatics::GetPlayerState(this, 0)->SetScore(UGameplayStatics::GetPlayerState(this, 0)->GetScore() + brickScore);
		UE_LOG(LogTemp, Warning, TEXT("%f"), UGameplayStatics::GetPlayerState(this, 0)->GetScore());
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), niagara, GetActorLocation(), GetActorRotation());

	meshComp->SetHiddenInGame(true);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//3�� �Ŀ� ������ �ı�
	FTimerHandle destroyTimer;
	GetWorldTimerManager().SetTimer(destroyTimer, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
		}), 2.0f, false);
}