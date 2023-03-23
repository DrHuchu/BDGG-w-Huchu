// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BrickBase.generated.h"

UCLASS()
class BDGG_API ABrickBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrickBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Block")
		class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category = "Block")
		class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, Category = "Block")
		class UGeometryCollection* DestructibleMesh;


};
