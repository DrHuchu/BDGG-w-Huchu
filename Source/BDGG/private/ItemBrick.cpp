// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBrick.h"

#include "Item.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void AItemBrick::AddScore()
{
	//Super::AddScore();

	brickHP--;
	mixValue += 1.0f / (brickMaxHP - 1);
	power -= 30.0f / brickMaxHP;

	meshComp->SetScalarParameterValueOnMaterials(FName("MixValue"), mixValue);
	meshComp->SetScalarParameterValueOnMaterials(FName("power"), power);

	if (brickHP == 0)
	{

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), niagara, GetActorLocation(), GetActorRotation());

		meshComp->SetHiddenInGame(true);
		meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//박스 위치에 아이템 스폰
		GetWorld()->SpawnActor<AItem>(itemFactory, GetActorLocation(), GetActorRotation());

		//3초 후에 완전히 파괴
		FTimerHandle destroyTimer;
		GetWorldTimerManager().SetTimer(destroyTimer, FTimerDelegate::CreateLambda([&]()
			{
				//UE_LOG(LogTemp, Warning, TEXT("Destroy"));
				Destroy();
			}), 2.0f, false);
	}
}
