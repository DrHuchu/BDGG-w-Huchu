// Fill out your copyright notice in the Description page of Project Settings.


#include "Unbreakable.h"

void AUnbreakable::AddScore()
{
	//Super::AddScore();

	meshComp->SetScalarParameterValueOnMaterials(FName("MixValue"), 1);
	meshComp->SetScalarParameterValueOnMaterials(FName("xbox"), 1);
}

void AUnbreakable::BeginPlay()
{
	Super::BeginPlay();

	meshComp->SetScalarParameterValueOnMaterials(FName("MixValue"), 0);
	meshComp->SetScalarParameterValueOnMaterials(FName("xbox"), 0);
}
