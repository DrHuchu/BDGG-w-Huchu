// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "BDGGGameInstance.generated.h"

USTRUCT()
struct FSessionInfo
{
	GENERATED_BODY()

	FString roomName;
	int32 currentPlayers;
	int32 maxPlayers;
	int32 ping;
	int32 idx;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSearchResult, FSessionInfo, sessionInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSearchFinished);

UCLASS()
class BDGG_API UBDGGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	UBDGGGameInstance();
	FName sessionID;
	IOnlineSessionPtr sessionInterface;
	TSharedPtr<FOnlineSessionSearch> sessionSearch;
	FOnSearchResult searchResultDele;
	FOnSearchFinished searchFinishedDele;

	UFUNCTION()
	void OnCreationSessionComplete(FName sessionName, bool bIsSuccess);
	UFUNCTION()
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName sessionNameJoined, enum  EOnJoinSessionCompleteResult::Type joinResult);

	void CreateMySession(FString roomName, int playerCount);
	void FindMySession();
	void JoinMySession(int sessionIndex);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGameModeWidget> gameModeWidgetFactory;
	UPROPERTY(BlueprintReadOnly)
	class UGameModeWidget* gameModeWidgetUI;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ULobbyWidget> lobbyWidgetFactory;
	UPROPERTY(BlueprintReadOnly)
	class ULobbyWidget* lobbyWidgetUI;
};
