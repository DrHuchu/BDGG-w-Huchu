// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/StaticMeshComponent.h>

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	//�浹ü
	//1)�浹ü ����ϱ�
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	//2)�浹�������� ����
	sphereComp->SetCollisionProfileName(TEXT("BlockAll"));
	//3)�浹ü ũ�� ����
	sphereComp->SetSphereRadius(13);
	//4)��Ʈ�� ���
	SetRootComponent(sphereComp);

	//�ܰ�
	//1)�ܰ� ����ϱ�
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	//2)�θ�������Ʈ ����
	meshComp->SetupAttachment(RootComponent);
	//3)�浹 ��Ȱ��ȭ
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//meshComp->SetCollisionProfileName(TEXT("NoCollision)); �ص��ǰ� ����ó���ص�����
	//4)�ܰ�ũ�⼳��
	meshComp->SetRelativeScale3D(FVector(0.25f));

	//�̵�������Ʈ
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("movementComp"));
	//movement ������Ʈ�� ���Ž�ų ������Ʈ ����
	movementComp->SetUpdatedComponent(sphereComp);
	movementComp->InitialSpeed = 5000;
	movementComp->MaxSpeed = 5000;
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.5f;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle dieTimerHandle;

	GetWorldTimerManager().SetTimer(dieTimerHandle, FTimerDelegate::CreateLambda(
		[this]()->void {
			this->Destroy();
		}
	), 0.05, false);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::OnDie()
{
	Destroy();
}

