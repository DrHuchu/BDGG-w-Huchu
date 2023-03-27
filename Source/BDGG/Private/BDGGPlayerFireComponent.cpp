// Fill out your copyright notice in the Description page of Project Settings.


#include "BDGGPlayerFireComponent.h"
#include "BDGGPlayer.h"

// Sets default values for this component's properties
UBDGGPlayerFireComponent::UBDGGPlayerFireComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USoundBase> tempFireSound(TEXT("/Script/Engine.SoundWave'/Game/SniperGun/Rifle.Rifle'"));

	if (tempFireSound.Succeeded())
	{
		fireSound = tempFireSound.Object;
	}
}


// Called when the game starts
void UBDGGPlayerFireComponent::BeginPlay()
{
	Super::BeginPlay();

	// UI�� �����ϰ�ʹ�.
	crosshairUI = CreateWidget(GetWorld(), crosshairFactory);
	sniperUI = CreateWidget(GetWorld(), sniperFactory);
	// crosshairUI�� ȭ�鿡 ǥ���ϰ�ʹ�.
	crosshairUI->AddToViewport();

	ChooseGun(GRENADE_GUN);

	gunAmmo = maxGunAmmo;
	sniperAmmo = maxSniperAmmo;
	
}


// Called every frame
void UBDGGPlayerFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	me = Cast<ABDGGPlayer>(GetOwner());
}

void UBDGGPlayerFireComponent::SetupPlayerInput(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UBDGGPlayerFireComponent::OnActionFirePressed);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &UBDGGPlayerFireComponent::OnActionFireReleased);

	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &UBDGGPlayerFireComponent::OnActionGrenade);

	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &UBDGGPlayerFireComponent::OnActionSniper);

	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &UBDGGPlayerFireComponent::OnActionZoomIn);

	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &UBDGGPlayerFireComponent::OnActionZoomOut);

	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &UBDGGPlayerFireComponent::OnActionReload);
}



void UBDGGPlayerFireComponent::OnMyGunReload()
{
	gunAmmo = maxGunAmmo;
	me->OnMyGrenadGunAmmoUpdate(gunAmmo, maxGunAmmo);
}

void UBDGGPlayerFireComponent::OnMySniperReload()
{
	sniperAmmo = maxSniperAmmo;
	me->OnMySniperGunAmmoUpdate(sniperAmmo, maxSniperAmmo);
}


void UBDGGPlayerFireComponent::OnActionFirePressed()
{
	// ���� �� �Ѿ��� �����ִ��� �����ϰ�ʹ�.
	// ���� �����ִٸ� 1�� �����ϰ�ʹ�.
	// �׷��� ������ ���� ���� �ʰڴ�...
	if (bChooseGrenadeGun)
	{
		if (gunAmmo > 0)
		{
			gunAmmo--;
			me->OnMyGrenadGunAmmoUpdate(gunAmmo, maxGunAmmo);
		}
		else { return; }
	}
	else
	{
		if (sniperAmmo > 0)
		{
			sniperAmmo--;
			me->OnMySniperGunAmmoUpdate(sniperAmmo, maxSniperAmmo);
		}
		else { return; }
	}


	// ī�޶� ����ʹ�.
	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	// ���� �̹� ���� �־��ٸ�
	// -> ���� canShakeInstance�� nullptr�� �ƴϴ�. �׸��� ���� ���̶��
	if (nullptr != canShakeInstance && false == canShakeInstance->IsFinished())
	{
		// ����ϰ�
		cameraManager->StopCameraShake(canShakeInstance);
	}
	// ����ʹ�.
	canShakeInstance = cameraManager->StartCameraShake(camShakeFactory);


	// �ѽ�� �ִϸ��̼��� ����ϰ�ʹ�.
	//auto anim = Cast<UBDGGPlayerAnim>(me->GetMesh()->GetAnimInstance());
	//anim->OnFire(TEXT("Default"));

	// �ѼҸ��� ����ʹ�.
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireSound, me->GetActorLocation(), me->GetActorRotation());

	// ���� �⺻���̶��
	if (bChooseGrenadeGun)
	{
		me->GetWorldTimerManager().SetTimer(fireTimerHandle, this, &UBDGGPlayerFireComponent::DoFire, fireInterval, true);

		DoFire();
	}
	// �׷����ʴٸ� (�������۰�)
	else
	{
		FHitResult hitInfo;
		FVector start = me->cameraComp->GetComponentLocation(); // ī�޶��� ������ǥ
		FVector end = start + me->cameraComp->GetForwardVector() * 100000;
		FCollisionQueryParams params;
		params.AddIgnoredActor(me);

		// �ٶ󺸰�ʹ�.
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECollisionChannel::ECC_Visibility, params);

		// ���� �ε��� ���� �ִٸ�
		if (true == bHit)
		{
			// ��ȣ�ۿ��� �ϰ�ʹ�.

			// �ε��� ���� ��������Ʈ�� ǥ���ϰ�ʹ�.
			FTransform trans(hitInfo.ImpactPoint);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletImpactFactory, trans);

			// ���� �ε��� ���Ͱ� Enemy���
			auto hitActor = hitInfo.GetActor();
			AEnemy* enemy = Cast<AEnemy>(hitActor);
			if (nullptr != enemy)
			{
				// Enemy���� �������� �ְ�ʹ�.
				UEnemyFSM* fsm = Cast<UEnemyFSM>(enemy->GetDefaultSubobjectByName(TEXT("enemyFSM")));

				fsm->OnDamageProcess(1);


			}

			auto hitComp = hitInfo.GetComponent();
			// �ε��� ��ü�� �����ۿ��� �ϰ��ִٸ�
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				// ���� ���ϰ�ʹ�.
				FVector forceDir = (hitInfo.TraceEnd - hitInfo.TraceStart).GetSafeNormal();

				FVector force = forceDir * 1000000 * hitComp->GetMass();

				hitComp->AddForce(force);
			}
		}




	}
}

void UBDGGPlayerFireComponent::OnActionFireReleased()
{
	me->GetWorldTimerManager().ClearTimer(fireTimerHandle);
}

void UBDGGPlayerFireComponent::DoFire()
{
	// �÷��̾� 1M��

	FTransform t = me->gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	//t.SetRotation(FQuat(GetControlRotation()));

	GetWorld()->SpawnActor<ABullet>(bulletFactory, t);

}

void UBDGGPlayerFireComponent::ChooseGun(bool bGrenade)
{
	// ���� �ٲٱ� ���� �������۰��̴� �׸��� �ٲٷ��°��� ��ź�̸�
	if (false == bChooseGrenadeGun && true == bGrenade) {
		// FOV�� 90, cui O sui X
		me->cameraComp->SetFieldOfView(90);
		crosshairUI->AddToViewport();
		sniperUI->RemoveFromParent();
	}

	bChooseGrenadeGun = bGrenade;

	me->gunMeshComp->SetVisibility(bGrenade);
	// Not ��Ʈ����
	me->sniperMeshComp->SetVisibility(!bGrenade);

	me->OnMyChooseGun(bChooseGrenadeGun);
}

void UBDGGPlayerFireComponent::OnActionGrenade()
{
	ChooseGun(GRENADE_GUN);
}

void UBDGGPlayerFireComponent::OnActionSniper()
{
	ChooseGun(SNIPER_GUN);
}

void UBDGGPlayerFireComponent::OnActionZoomIn()
{
	// ���� ��ź�̶�� �ٷ� ����
	if (true == bChooseGrenadeGun) {
		return;
	}
	// Ȯ�� FOV 30
	me->cameraComp->SetFieldOfView(30);
	// crosshair�� �Ⱥ��̰��ϰ�, Ȯ����� ���̰��ϰ�ʹ�.
	crosshairUI->RemoveFromParent();
	sniperUI->AddToViewport();
}

void UBDGGPlayerFireComponent::OnActionZoomOut()
{
	// ���� ��ź�̶�� �ٷ� ����
	if (true == bChooseGrenadeGun) {
		return;
	}
	// Ȯ�� FOV 90
	me->cameraComp->SetFieldOfView(90);
	// crosshair�� ���̰��ϰ�, Ȯ����� �Ⱥ��̰��ϰ�ʹ�.
	crosshairUI->AddToViewport();
	sniperUI->RemoveFromParent();
}

