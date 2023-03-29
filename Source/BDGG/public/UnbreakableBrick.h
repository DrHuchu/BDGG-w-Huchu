// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnbreakableBrick.generated.h"

UCLASS()
class BDGG_API AUnbreakableBrick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnbreakableBrick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Brick")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category = "Brick")
	class UBoxComponent* boxComp;

};
