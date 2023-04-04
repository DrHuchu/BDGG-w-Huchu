// Fill out your copyright notice in the Description page of Project Settings.


#include "BDGGPlayer_AnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "BDGGPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"


void UBDGGPlayer_AnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	me = Cast<ABDGGPlayer>(GetOwningActor());
}

void UBDGGPlayer_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//애니메이션 미리보기 예외 처리
	if (me == nullptr)
	{
		return;
	}
	moveSpeed = me->GetVelocity().Length();



	/*
	//FVector forwarVec = me->GetActorForwardVector();
	FVector forwarVec = FRotationMatrix(me->GetActorRotation().GetScaledAxis(EAxis::X));
	FVector normalVelocity = me->GetVelocity().GetSafeNormal2D();

	//회전 각도 계산
	double cosTheta = FVector::DotProduct(forwarVec, normalVelocity);
	double rad = FMath::Acos(cosTheta);
	float degree = FMath::RadiansToDegrees(rad);
	FVector rightVec = FRotationMatrix(me->GetActorRotation()).GetUnitAxis(EAxis::Y);

	//회전 방향 계산
	cosTheta = FVector::DotProduct(rightVec, normalVelocity);
	rotAngle = cosTheta >=0 ? degree : degree * -1.0f;
	*/

	//rotAngle = CalculateDirection(me->GetVelocity(), me->GetActorRotation());
	rotAngle = UKismetAnimationLibrary::CalculateDirection(me->GetVelocity(), me->GetActorRotation());

	//bIsJumping = me->GetCharacterMovement()->IsFalling();

	//FRotator viewRot = me->GetBaseAimRotation();
	//FRotator playerRot = me->GetActorRotation();
	//FRotator deltaRot = playerRot - viewRot;
	//pitch = deltaRot.GetNormalized().Pitch;

	//UE_LOG(LogTemp, Warning, TEXT("movespeed is %f"), moveSpeed);
	//UE_LOG(LogTemp, Warning, TEXT("pitch is %f"), pitch);
}


void UBDGGPlayer_AnimInstance::OnFire()
{
	Montage_Play(fireMontageFactory);
}
