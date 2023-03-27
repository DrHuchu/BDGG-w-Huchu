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

	// UI를 생성하고싶다.
	crosshairUI = CreateWidget(GetWorld(), crosshairFactory);
	sniperUI = CreateWidget(GetWorld(), sniperFactory);
	// crosshairUI를 화면에 표시하고싶다.
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
	// 총을 쏠때 총알이 남아있는지 검증하고싶다.
	// 만약 남아있다면 1발 차감하고싶다.
	// 그렇지 않으면 총을 쏘지 않겠다...
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


	// 카메라를 흔들고싶다.
	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	// 만약 이미 흔들고 있었다면
	// -> 만약 canShakeInstance가 nullptr이 아니다. 그리고 흔드는 중이라면
	if (nullptr != canShakeInstance && false == canShakeInstance->IsFinished())
	{
		// 취소하고
		cameraManager->StopCameraShake(canShakeInstance);
	}
	// 흔들고싶다.
	canShakeInstance = cameraManager->StartCameraShake(camShakeFactory);


	// 총쏘는 애니메이션을 재생하고싶다.
	//auto anim = Cast<UBDGGPlayerAnim>(me->GetMesh()->GetAnimInstance());
	//anim->OnFire(TEXT("Default"));

	// 총소리를 내고싶다.
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireSound, me->GetActorLocation(), me->GetActorRotation());

	// 만약 기본총이라면
	if (bChooseGrenadeGun)
	{
		me->GetWorldTimerManager().SetTimer(fireTimerHandle, this, &UBDGGPlayerFireComponent::DoFire, fireInterval, true);

		DoFire();
	}
	// 그렇지않다면 (스나이퍼건)
	else
	{
		FHitResult hitInfo;
		FVector start = me->cameraComp->GetComponentLocation(); // 카메라의 월드좌표
		FVector end = start + me->cameraComp->GetForwardVector() * 100000;
		FCollisionQueryParams params;
		params.AddIgnoredActor(me);

		// 바라보고싶다.
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECollisionChannel::ECC_Visibility, params);

		// 만약 부딪힌 것이 있다면
		if (true == bHit)
		{
			// 상호작용을 하고싶다.

			// 부딪힌 곳에 폭발이펙트를 표시하고싶다.
			FTransform trans(hitInfo.ImpactPoint);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletImpactFactory, trans);

			// 만약 부딪힌 액터가 Enemy라면
			auto hitActor = hitInfo.GetActor();
			AEnemy* enemy = Cast<AEnemy>(hitActor);
			if (nullptr != enemy)
			{
				// Enemy에게 데미지를 주고싶다.
				UEnemyFSM* fsm = Cast<UEnemyFSM>(enemy->GetDefaultSubobjectByName(TEXT("enemyFSM")));

				fsm->OnDamageProcess(1);


			}

			auto hitComp = hitInfo.GetComponent();
			// 부딪힌 물체가 물리작용을 하고있다면
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				// 힘을 가하고싶다.
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
	// 플레이어 1M앞

	FTransform t = me->gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	//t.SetRotation(FQuat(GetControlRotation()));

	GetWorld()->SpawnActor<ABullet>(bulletFactory, t);

}

void UBDGGPlayerFireComponent::ChooseGun(bool bGrenade)
{
	// 만약 바꾸기 전이 스나이퍼건이다 그리고 바꾸려는것이 유탄이면
	if (false == bChooseGrenadeGun && true == bGrenade) {
		// FOV를 90, cui O sui X
		me->cameraComp->SetFieldOfView(90);
		crosshairUI->AddToViewport();
		sniperUI->RemoveFromParent();
	}

	bChooseGrenadeGun = bGrenade;

	me->gunMeshComp->SetVisibility(bGrenade);
	// Not 비트연산
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
	// 만약 유탄이라면 바로 종료
	if (true == bChooseGrenadeGun) {
		return;
	}
	// 확대 FOV 30
	me->cameraComp->SetFieldOfView(30);
	// crosshair를 안보이게하고, 확대경을 보이게하고싶다.
	crosshairUI->RemoveFromParent();
	sniperUI->AddToViewport();
}

void UBDGGPlayerFireComponent::OnActionZoomOut()
{
	// 만약 유탄이라면 바로 종료
	if (true == bChooseGrenadeGun) {
		return;
	}
	// 확대 FOV 90
	me->cameraComp->SetFieldOfView(90);
	// crosshair를 보이게하고, 확대경을 안보이게하고싶다.
	crosshairUI->AddToViewport();
	sniperUI->RemoveFromParent();
}

