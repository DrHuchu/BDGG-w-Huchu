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

	//�Ѿ� ������ �̺�Ʈ
	meshComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::BulletCrash);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�ӽ� �ڵ� ����--------------------------------------
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * 200 * DeltaTime);
}

// �Ѿ� �浹 �Լ�
void ABullet::BulletCrash(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult)
{
	//������ �� ���Ͱ� �����̶��
	brick = Cast<ABrickBase>(OtherActor);
	unbreakable = Cast<AUnbreakable>(OtherActor);

	if(unbreakable)
	{
		unbreakable->OnBlockHit();
		Destroy();
	}

	if(brick)
	{
	//���� �ı��ϰ�
	brick->hitOrigin = GetActorLocation();
	brick->hitDirection = brick->GetActorLocation() - GetActorLocation();
	brick->SetOwner(GetOwner());
	brick->OnBlockHit();
	//�ڱ� �ڽŵ� �ı��Ѵ�.
	Destroy();
	}

	Destroy();

}

