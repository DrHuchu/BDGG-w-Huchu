// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/ActorComponent.h"
#include "BDGGPlayer_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BDGG_API UBDGGPlayer_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class ABDGGPlayer* me;

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimSettings)
	float rotAngle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimSettings)
	float moveSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimSettings)
	bool bIsJumping = false;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimSettings)
	float pitch = 0;
	
	UPROPERTY(EditAnywhere)
	class UAnimMontage* fireMontageFactory;

	UFUNCTION(BlueprintCallable)
	void OnFire();
};
