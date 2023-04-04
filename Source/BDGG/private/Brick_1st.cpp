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
	
	brickHP--;
	mixValue += 1.0f / (brickMaxHP -1);
	power -= 30.0f / brickMaxHP;

	meshComp->SetScalarParameterValueOnMaterials(FName("MixValue"), mixValue);
	meshComp->SetScalarParameterValueOnMaterials(FName("power"), power);

	if(brickHP == 0)
	{
		auto owningPawn = Cast<APawn>(GetOwner());

		auto ps = Cast<ABDGGPlayerState>(owningPawn->GetPlayerState());
		if(ps)
		{
			ps->SetScore(ps->GetScore() + brickScore);
		}

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), niagara, GetActorLocation(), GetActorRotation());

		meshComp->SetHiddenInGame(true);
		meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


		if (owningPawn && owningPawn->GetController() && owningPawn->GetController()->IsLocalController())
		{
			scoreWidget->SetVisibility(true);
			scoreWidget->SetComponentTickEnabled(true);
		}
		
		//3초 후에 완전히 파괴
		FTimerHandle destroyTimer;
		GetWorldTimerManager().SetTimer(destroyTimer, FTimerDelegate::CreateLambda([&]()
			{
				//UE_LOG(LogTemp, Warning, TEXT("Destroy"));
				Destroy();
			}), 2.0f, false);
	}
}
