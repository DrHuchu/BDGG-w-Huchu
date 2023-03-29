// Fill out your copyright notice in the Description page of Project Settings.


#include "BDGGPlayerMoveComponent.h"
#include "BDGGPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "KismetAnimationLibrary.h"
#include "GameFramework/Character.h"


// Sets default values for this component's properties
UBDGGPlayerMoveComponent::UBDGGPlayerMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}

// Called when the game starts
void UBDGGPlayerMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<ABDGGPlayer>(GetOwner());

	// �¾ �� �ӷ��� speedWalk�� �����ϰ�ʹ�.
	me->GetCharacterMovement()->MaxWalkSpeed = speedWalk;
}


// Called every frame
void UBDGGPlayerMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// direction �������� �̵��ϰ�ʹ�.
	FTransform trans(me->GetControlRotation());
	FVector resultDirection = trans.TransformVector(direction);
	resultDirection.Z = 0;
	resultDirection.Normalize();
	me->AddMovementInput(resultDirection);
	direction = FVector::ZeroVector;
}

void UBDGGPlayerMoveComponent::SetupPlayerInput(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &UBDGGPlayerMoveComponent::OnAxisHorizontal);

	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &UBDGGPlayerMoveComponent::OnAxisVertical);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &UBDGGPlayerMoveComponent::OnAxisLookUp);

	PlayerInputComponent->BindAxis(TEXT("TurnRight"), this, &UBDGGPlayerMoveComponent::OnAxisTurnRight);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &UBDGGPlayerMoveComponent::OnActionJump);

	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &UBDGGPlayerMoveComponent::OnActionRunPressed);

	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &UBDGGPlayerMoveComponent::OnActionRunReleased);

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &UBDGGPlayerMoveComponent::OnActionCrouchPressed);

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &UBDGGPlayerMoveComponent::OnActionCrouchReleased);
}


void UBDGGPlayerMoveComponent::OnAxisHorizontal(float value)
{  
	
	direction.Y = value;
}

void UBDGGPlayerMoveComponent::OnAxisVertical(float value)
{
	
	direction.X = value;
}

void UBDGGPlayerMoveComponent::OnAxisLookUp(float value)
{
	
	me->AddControllerPitchInput(value);
}

void UBDGGPlayerMoveComponent::OnAxisTurnRight(float value)
{
	
	me->AddControllerYawInput(value);
}

void UBDGGPlayerMoveComponent::OnActionJump()
{
	me->Jump();
}

void UBDGGPlayerMoveComponent::OnActionRunPressed()
{
	me->GetCharacterMovement()->MaxWalkSpeed = speedRun;
}
void UBDGGPlayerMoveComponent::OnActionRunReleased()
{
	me->GetCharacterMovement()->MaxWalkSpeed = speedWalk;
}
void UBDGGPlayerMoveComponent::OnActionCrouchPressed()
{
	me->GetCharacterMovement()->MaxWalkSpeedCrouched = speedCrouch;
	me->Crouch();
}
void UBDGGPlayerMoveComponent::OnActionCrouchReleased()
{
	me->GetCharacterMovement()->MaxWalkSpeedCrouched = speedWalk;
	me->UnCrouch();
}

void UBDGGPlayerMoveComponent::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	me = Cast<ABDGGPlayer>(GetOwningActor());
}

void UBDGGPlayerMoveComponent::NativeUpdateAnimation(float DeltaSeconds)
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

	//rotAngle = CalculateDirection(me->GetVelocity(), me->GetActorRotation());
	rotAngle = UKismetAnimationLibrary::CalculateDirection(me->GetVelocity(), me->GetActorRotation());

	bIsJumping = me->GetCharacterMovement()->IsFalling();

	FRotator viewRot = me->GetBaseAimRotation();
	FRotator playerRot = me->GetActorRotation();
	FRotator deltaRot = playerRot - viewRot;
	pitch = deltaRot.GetNormalized().pitch;

}
