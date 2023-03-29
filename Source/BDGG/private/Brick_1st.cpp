// Fill out your copyright notice in the Description page of Project Settings.


#include "Brick_1st.h"

#include "BDGGGameMode.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/GameStateBase.h"
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
		UE_LOG(LogTemp, Warning, TEXT("Score ++*3"));
		if(gm)
		{
			//���� ����
			UGameplayStatics::GetPlayerState(this, 0)->SetScore(UGameplayStatics::GetPlayerState(this, 0)->GetScore() + brickScore1);
			UE_LOG(LogTemp, Warning, TEXT("%f"), UGameplayStatics::GetPlayerState(this, 0)->GetScore());

		}
		

		meshComp->SetHiddenInGame(true);
		meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		AFieldSystemActor* field = GetWorld()->SpawnActor<class AFieldSystemActor>(masterField, destructibleMesh->GetComponentLocation(), destructibleMesh->GetComponentRotation());
		destructibleMesh->SetVisibility(true);
		destructibleMesh->SetSimulatePhysics(true);
		destructibleMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		destructibleMesh->SetCollisionProfileName(FName("Destructible"));

		destructibleMesh->AddForceAtLocation(hitDirection.GetSafeNormal() * 5000000.0f , GetActorLocation());


		//3�� �Ŀ� ������ �ı�
		FTimerHandle destroyTimer;
		GetWorldTimerManager().SetTimer(destroyTimer, FTimerDelegate::CreateLambda([&]()
			{
				//UE_LOG(LogTemp, Warning, TEXT("Destroy"));
				Destroy();
			}), 3.0f, false);
	}
}
