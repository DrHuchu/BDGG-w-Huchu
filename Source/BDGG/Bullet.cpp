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

	
	//충돌체
	//1)충돌체 등록하기
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	//2)충돌프로파일 설정
	sphereComp->SetCollisionProfileName(TEXT("BlockAll"));
	//3)충돌체 크기 설정
	sphereComp->SetSphereRadius(13);
	//4)루트로 등록
	SetRootComponent(sphereComp);

	//외관
	//1)외관 등록하기
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	//2)부모컴포넌트 지정
	meshComp->SetupAttachment(RootComponent);
	//3)충돌 비활성화
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//meshComp->SetCollisionProfileName(TEXT("NoCollision)); 해도되고 위에처럼해도도고
	//4)외관크기설정
	meshComp->SetRelativeScale3D(FVector(0.25f));

	//이동컴포넌트
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("movementComp"));
	//movement 컴포넌트가 갱신시킬 컴포넌트 지정
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

