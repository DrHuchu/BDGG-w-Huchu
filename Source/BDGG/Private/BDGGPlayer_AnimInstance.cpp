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

	//�ִϸ��̼� �̸����� ���� ó��
	if (me == nullptr)
	{
		return;
	}
	moveSpeed = me->GetVelocity().Length();
	

	


	/*
	//FVector forwarVec = me->GetActorForwardVector();
	FVector forwarVec = FRotationMatrix(me->GetActorRotation().GetScaledAxis(EAxis::X));
	FVector normalVelocity = me->GetVelocity().GetSafeNormal2D();

	//ȸ�� ���� ���
	double cosTheta = FVector::DotProduct(forwarVec, normalVelocity);
	double rad = FMath::Acos(cosTheta);
	float degree = FMath::RadiansToDegrees(rad);
	FVector rightVec = FRotationMatrix(me->GetActorRotation()).GetUnitAxis(EAxis::Y);

	//ȸ�� ���� ���
	cosTheta = FVector::DotProduct(rightVec, normalVelocity);
	rotAngle = cosTheta >=0 ? degree : degree * -1.0f;
	*/

	
	rotAngle = UKismetAnimationLibrary::CalculateDirection(me->GetVelocity(), me->GetActorRotation());

	bIsJumping = me->GetCharacterMovement()->IsFalling();



	FRotator viewRot = me->GetBaseAimRotation();
	FRotator playerRot = me->GetActorRotation();
	FRotator deltaRot = playerRot - viewRot;
	pitch = FMath::Clamp(deltaRot.GetNormalized().Pitch, -45.0f, 45.0f);


}


void UBDGGPlayer_AnimInstance::OnFire()
{
	Montage_Play(fireMontageFactory);
}
