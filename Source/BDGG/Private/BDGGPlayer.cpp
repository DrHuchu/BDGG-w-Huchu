// Fill out your copyright notice in the Description page of Project Settings.


#include "BDGGPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "BDGGPlayerMoveComponent.h"
#include "Bullet.h"
#include "BDGGPlayer_AnimInstance.h"

// Sets default values
ABDGGPlayer::ABDGGPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// sudo code 의사코드 => 알고리즘
	// 1. 외관에 해당하는 에셋을 읽어오고싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	// 2. 읽어왔을때 성공했다면
	if (tempMesh.Succeeded())
	{
		// 3. Mesh에 적용하고싶다. 
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		// 4. Transform 을 수정하고싶다.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// 스프링암, 카메라 컴포넌트를 생성하고싶다.
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArmComp"));
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	// 스프링암을 루트에 붙이고
	springArmComp->SetupAttachment(RootComponent);
	// 카메라는 스프링암에 붙이고싶다.
	cameraComp->SetupAttachment(springArmComp);

	springArmComp->SetRelativeLocation(FVector(0, 50, 100));
	springArmComp->TargetArmLength = 250;

	// 입력값을 회전에 반영하고싶다.
	bUseControllerRotationYaw = true;
	springArmComp->bUsePawnControlRotation = true;
	cameraComp->bUsePawnControlRotation = true;


	// 일반총의 컴포넌트를 만들고싶다.
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	// 일반총의 에셋을 읽어서 컴포넌트에 넣고싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (tempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(tempGunMesh.Object);
		gunMeshComp->SetRelativeLocationAndRotation(FVector(-9, -2, -6), FRotator(0, 100, -20));
	} 

	// 이동컴포넌트와 총쏘기컴포넌트를 생성하고싶다.
	moveComp = CreateDefaultSubobject<UBDGGPlayerMoveComponent>(TEXT("moveComp"));

}

// Called when the game starts or when spawned
void ABDGGPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABDGGPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABDGGPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	moveComp->SetupPlayerInput(PlayerInputComponent);
	//setupInputDelegate.Broadcast(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ABDGGPlayer::OnActionFirePressed);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ABDGGPlayer::OnActionFireReleased);

}

void ABDGGPlayer::OnActionFirePressed()
{
	DoFire();
		//총쏘는 애니메이션을 사용하고싶다
	auto anim = Cast<UBDGGPlayer_AnimInstance>(GetMesh()->GetAnimInstance());
	anim->OnFire();
}

void ABDGGPlayer::OnActionFireReleased()
{

}

void ABDGGPlayer::DoFire()
{
	//SpawnActor
	//이런거 찾을 때 APlayer Getworld UKismetMathLibrary, UGameplayStatics
	//중에 찾아보기
	//플레이어 1m 앞
	UE_LOG(LogTemp, Warning, TEXT("shooting"));
	FTransform t = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ABullet>(bulletFactory, t);
}

