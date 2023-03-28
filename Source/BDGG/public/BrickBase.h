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

	UPROPERTY(EditAnywhere, Category = "Brick")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category = "Brick")
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brick")
	class UGeometryCollectionComponent* destructibleMesh;


	UPROPERTY(EditAnywhere, Category = "Brick")
	FVector hitDirection;

	UPROPERTY(EditAnywhere, Category = "Brick")
	FVector hitOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brick")
	TSubclassOf<class AFieldSystemActor> masterField;

	UFUNCTION()
	void OnBlockHit();

	UFUNCTION()
	virtual void AddScore();

	class ABDGGGameMode* gm;

	UPROPERTY(VisibleAnywhere, Category = "Brick")
	bool bIsBroken = false;

	UFUNCTION()
	void ParticleDownsize(UPrimitiveComponent* ChangedComponent, EComponentPhysicsStateChange StateChange);
};
