// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class BDGG_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY()
	class UBDGGGameInstance* gi;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* text_LobbyName1;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* text_LobbyName2;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* text_LobbyName3;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* text_LobbyName4;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_GameStart;
	UPROPERTY()
	TArray<UTextBlock*> lobbyNameArray;

	UFUNCTION()
	void LobbyGameStart();
	UFUNCTION()
	void RefreshLobbyName();
};
