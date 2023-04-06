// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "BrickBase.h"
#include "Unbreakable.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	SetRootComponent(meshComp);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	//총알 오버랩 이벤트
	meshComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::BulletCrash);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//임시 자동 전진--------------------------------------
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * 200 * DeltaTime);
}

// 총알 충돌 함수
void ABullet::BulletCrash(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	//오버랩 된 액터가 벽돌이라면
	brick = Cast<ABrickBase>(OtherActor);
	unbreakable = Cast<AUnbreakable>(OtherActor);

	if(unbreakable)
	{
		unbreakable->OnBlockHit();
		Destroy();
	}

	if(brick)
	{
	//블럭을 파괴하고
	brick->hitOrigin = GetActorLocation();
	brick->hitDirection = brick->GetActorLocation() - GetActorLocation();
	brick->SetOwner(GetOwner());
	brick->OnBlockHit();
	//자기 자신도 파괴한다.
	Destroy();
	}

	Destroy();

}

