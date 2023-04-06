// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BDGGPlayer.generated.h"

UCLASS()
class BDGG_API ABDGGPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABDGGPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnActionFirePressed();
	void OnActionFireReleased();

	void DoFire();

	//마우스 왼쪽버튼을누르면 총알공장에서 총알을만들어서 
	//총구위치에 배치하고싶다.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> bulletFactory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCameraComponent* cameraComp;

	UPROPERTY(EditAnywhere)
	class UBDGGPlayerMoveComponent* moveComp;

	UFUNCTION(Server, Unreliable)
	void DoFireServer();

	UFUNCTION(NetMulticast, Unreliable)
	void DoFireMulticast();

	UPROPERTY(EditAnywhere)
	class USoundBase* fireSound;

	UFUNCTION(Client, Unreliable)
	void SpawnFireSound();
};
