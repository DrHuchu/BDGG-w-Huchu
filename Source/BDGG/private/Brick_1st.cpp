// Fill out your copyright notice in the Description page of Project Settings.


#include "Brick_1st.h"

#include "BDGGGameMode.h"
#include "BDGGPlayerState.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/PrimitiveComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void ABrick_1st::AddScore_Implementation()
{
	//Super::AddScore();
	
	ChangeColor();

	if(brickHP == 0)
	{
		auto owningPawn = Cast<APawn>(GetOwner());

		if (owningPawn == nullptr)
		{
			return;
		}

		auto ps = Cast<ABDGGPlayerState>(owningPawn->GetPlayerState());
		if(ps)
		{
			ps->SetScore(ps->GetScore() + brickScore1);
		}

		//나이아가라 스폰, 블럭 안보이게 처리
		SpawnFX();
	}
}

void ABrick_1st::ChangeColor_Implementation()
{
	brickHP--;
	mixValue += 1.0f / (brickMaxHP - 1);
	power -= 30.0f / brickMaxHP;

	meshComp->SetScalarParameterValueOnMaterials(FName("MixValue"), mixValue);
	meshComp->SetScalarParameterValueOnMaterials(FName("power"), power);
}
