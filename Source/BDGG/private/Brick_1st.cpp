// Fill out your copyright notice in the Description page of Project Settings.


#include "Brick_1st.h"

#include "BDGGGameMode.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/PrimitiveComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void ABrick_1st::AddScore()
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

		UE_LOG(LogTemp, Warning, TEXT("Score ++*3"));
		if(gm)
		{
			//점수 득점
			UGameplayStatics::GetPlayerState(this, 0)->SetScore(UGameplayStatics::GetPlayerState(this, 0)->GetScore() + brickScore1);
			UE_LOG(LogTemp, Warning, TEXT("%f"), UGameplayStatics::GetPlayerState(this, 0)->GetScore());
		}

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), niagara, GetActorLocation(), GetActorRotation());

		meshComp->SetHiddenInGame(true);
		meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


		if (owningPawn->GetController() && owningPawn->GetController()->IsLocalController())
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
