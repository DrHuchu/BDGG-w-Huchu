// Fill out your copyright notice in the Description page of Project Settings.


#include "BDGGPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "BDGGPlayerMoveComponent.h"
#include "Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "BDGGPlayer_AnimInstance.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ABDGGPlayer::ABDGGPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// sudo code �ǻ��ڵ� => �˰�����
	// 1. �ܰ��� �ش��ϴ� ������ �о�����ʹ�.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	// 2. �о������ �����ߴٸ�
	if (tempMesh.Succeeded())
	{
		// 3. Mesh�� �����ϰ��ʹ�. 
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		// 4. Transform �� �����ϰ��ʹ�.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// ��������, ī�޶� ������Ʈ�� �����ϰ��ʹ�.
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArmComp"));
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	// ���������� ��Ʈ�� ���̰�
	springArmComp->SetupAttachment(RootComponent);
	// ī�޶�� �������Ͽ� ���̰��ʹ�.
	cameraComp->SetupAttachment(springArmComp);

	springArmComp->SetRelativeLocation(FVector(0, 50, 100));
	springArmComp->TargetArmLength = 250;

	// �Է°��� ȸ���� �ݿ��ϰ��ʹ�.
	bUseControllerRotationYaw = true;
	springArmComp->bUsePawnControlRotation = true;
	cameraComp->bUsePawnControlRotation = true;


	// �Ϲ����� ������Ʈ�� ������ʹ�.
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	// �Ϲ����� ������ �о ������Ʈ�� �ְ��ʹ�.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (tempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(tempGunMesh.Object);
		gunMeshComp->SetRelativeLocationAndRotation(FVector(-9, -2, -6), FRotator(0, 100, -20));
	} 

	// �̵�������Ʈ�� �ѽ��������Ʈ�� �����ϰ��ʹ�.
	moveComp = CreateDefaultSubobject<UBDGGPlayerMoveComponent>(TEXT("moveComp"));

	
}

// Called when the game starts or when spawned
void ABDGGPlayer::BeginPlay()
{
	Super::BeginPlay();

	crosshairUI = CreateWidget(GetWorld(), crosshairFactory);
	crosshairUI->AddToViewport();

	
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
	DoFireServer();

	//�ѽ�� �ִϸ��̼��� ����ϰ��ʹ�
	auto anim = Cast<UBDGGPlayer_AnimInstance>(GetMesh()->GetAnimInstance());
	anim->OnFire();
}

void ABDGGPlayer::OnActionFireReleased()
{

}

void ABDGGPlayer::DoFire()
{
	//SpawnActor
	//�̷��� ã�� �� APlayer Getworld UKismetMathLibrary, UGameplayStatics
	//�߿� ã�ƺ���
	//�÷��̾� 1m ��

	
	//���� �����۾��ٸ�
	if (itemnum == 0)
	{
	//����Ѿ��� ������ �ϰ��ʹ�
	
		FTransform t = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	//��
		t.SetRotation(GetControlRotation().Quaternion());
		ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, t);

		if (bullet)
		{
			bullet->SetOwner(this);
		}
	}
	//�������� �ִٸ� (itemnum != 0);
	else
	{
		//�Ķ��Ѿ��� ������ �ϰ��ʹ�.
		FTransform t = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		t.SetRotation(GetControlRotation().Quaternion());
		ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory2, t);

		if (bullet)
		{
			bullet->SetOwner(this);
		}
		//�Ķ��Ѿ��� �������� -1�� ����.
		itemnum = itemnum - 1;
	}
}

void ABDGGPlayer::SpawnFireSound_Implementation()
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), fireSound, GetActorLocation(), GetActorRotation());
}

void ABDGGPlayer::DoFireServer_Implementation()
{
	DoFire();
	SpawnFireSound();
}

