// Fill out your copyright notice in the Description page of Project Settings.


#include "Brick_3rd.h"

#include "BDGGGameMode.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/PrimitiveComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void ABrick_3rd::AddScore()
{
	//Super::AddScore();

	brickHP--;
	mixValue += 1.0f / (brickMaxHP - 1);
	power -= 30.0f / brickMaxHP;

	meshComp->SetScalarParameterValueOnMaterials(FName("MixValue"), mixValue);
	meshComp->SetScalarParameterValueOnMaterials(FName("power"), power);

	if (brickHP == 0)
	{
		auto owningPawn = Cast<APawn>(GetOwner());
		
		if (gm)
		{
			//���� ����
			UGameplayStatics::GetPlayerState(this, 0)->SetScore(UGameplayStatics::GetPlayerState(this, 0)->GetScore() + brickScore3);
			UE_LOG(LogTemp, Warning, TEXT("%f"), UGameplayStatics::GetPlayerState(this, 0)->GetScore());
		}


		meshComp->SetHiddenInGame(true);
		meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (owningPawn->GetController() && owningPawn->GetController()->IsLocalController())
		{
			scoreWidget->SetVisibility(true);
			scoreWidget->SetComponentTickEnabled(true);
		}

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), niagara, GetActorLocation(), GetActorRotation(), FVector(4.0f));

		//3�� �Ŀ� ������ �ı�
		FTimerHandle destroyTimer;
		GetWorldTimerManager().SetTimer(destroyTimer, FTimerDelegate::CreateLambda([&]()
			{
				//UE_LOG(LogTemp, Warning, TEXT("Destroy"));
				Destroy();
			}), 3.0f, false);
	}
}
