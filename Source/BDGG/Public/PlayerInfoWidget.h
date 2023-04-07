// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class BDGG_API UPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* text_name;

	void Setplayer(class ABDGGPlayer* player);

private:
	class ABDGGPlayer* me;
};
