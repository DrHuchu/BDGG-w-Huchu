// Fill out your copyright notice in the Description page of Project Settings.


#include "Brick_2nd.h"

#include "BDGGGameMode.h"
#include "BDGGPlayerState.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/PrimitiveComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void ABrick_2nd::AddScore_Implementation()
{
	//Super::AddScore();
	ChangeColor();

	if (brickHP == 0)
	{
		auto owningPawn = Cast<APawn>(GetOwner());

		if (owningPawn == nullptr)
		{
			return;
		}

		auto ps = Cast<ABDGGPlayerState>(owningPawn->GetPlayerState());
		if(ps)
		{
			ps->SetScore(ps->GetScore() + brickScore2);
		}

		//���̾ư��� ����, �� �Ⱥ��̰� ó��
		SpawnFX();
	}
}

void ABrick_2nd::ChangeColor_Implementation()
{
	brickHP--;
	mixValue += 1.0f / (brickMaxHP - 1);
	power -= 30.0f / brickMaxHP;

	meshComp->SetScalarParameterValueOnMaterials(FName("MixValue"), mixValue);
	meshComp->SetScalarParameterValueOnMaterials(FName("power"), power);
}
