// Fill out your copyright notice in the Description page of Project Settings.


#include "BDGGGameInstance.h"

#include "BDGGGameMode.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

UBDGGGameInstance::UBDGGGameInstance()
{
	sessionID = "BDGG";

	static ConstructorHelpers::FClassFinder<UGameModeWidget> gameModeWidgetTemp(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LTG/UI/WBP_GameModeWidget.WBP_GameModeWidget_C'"));
	if (gameModeWidgetTemp.Succeeded())
	{
		gameModeWidgetFactory = gameModeWidgetTemp.Class;
	}
}

void UBDGGGameInstance::Init()
{
	Super::Init();

	IOnlineSubsystem* subsys = IOnlineSubsystem::Get();
	if (subsys)
	{
		sessionInterface = subsys->GetSessionInterface();

		if (sessionInterface != nullptr)
		{
			sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UBDGGGameInstance::OnCreationSessionComplete);
			sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UBDGGGameInstance::OnFindSessionComplete);
			sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UBDGGGameInstance::OnJoinSessionComplete);
		}
	}

	FString platformName = subsys->GetSubsystemName().ToString();
	UE_LOG(LogTemp, Warning, TEXT("Platform Is : %s"), *platformName);

	gameModeWidgetUI = CreateWidget<UGameModeWidget>(GetWorld(), gameModeWidgetFactory);
}

void UBDGGGameInstance::OnCreationSessionComplete(FName sessionName, bool bIsSuccess)
{
	FString result = bIsSuccess ? TEXT("Create Session Success!") : TEXT("Create Session Failed!");
	UE_LOG(LogTemp, Warning, TEXT("%s : %s"), *result, *sessionName.ToString());

	if (bIsSuccess)
	{
		GetWorld()->ServerTravel("/Game/Maps/HuchuMap?Listen");
	}
}

void UBDGGGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TArray <FOnlineSessionSearchResult> searchResults = sessionSearch->SearchResults;
		UE_LOG(LogTemp, Warning, TEXT("Find Session count : %d"), searchResults.Num());

		for (int i = 0; i < searchResults.Num(); i++)
		{
			FSessionInfo searchedSessionInfo;

			FString roomName;
			searchResults[i].Session.SessionSettings.Get(FName("KEY_RoomName"), roomName);
			searchedSessionInfo.roomName = roomName;
			int32 maxPlayers = searchResults[i].Session.SessionSettings.NumPublicConnections;
			searchedSessionInfo.maxPlayers = maxPlayers;
			int32 currentPlayers = maxPlayers - searchResults[i].Session.NumOpenPublicConnections;
			searchedSessionInfo.currentPlayers = currentPlayers;
			int32 ping = searchResults[i].PingInMs;
			searchedSessionInfo.ping = ping;
			searchedSessionInfo.idx = i;

			searchResultDele.Broadcast(searchedSessionInfo);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Find Session Failed!!"));
	}

	searchFinishedDele.Broadcast();
}

void UBDGGGameInstance::OnJoinSessionComplete(FName sessionNameJoined, EOnJoinSessionCompleteResult::Type joinResult)
{
	if (joinResult == EOnJoinSessionCompleteResult::Success)
	{
		FString joinAddress;
		sessionInterface->GetResolvedConnectString(sessionNameJoined, joinAddress);
		UE_LOG(LogTemp, Warning, TEXT("Join Address : %s"), *joinAddress);

		APlayerController* pc = GetWorld()->GetFirstPlayerController();
		if (pc)
		{
			pc->ClientTravel(joinAddress, ETravelType::TRAVEL_Absolute);
		}
		/*
		ABDGGGameMode* gm = Cast<ABDGGGameMode>(GetWorld()->GetAuthGameMode());
		if (gm)
		{
			gm->StartWidgetPlay();
		}
		*/
	}
}

void UBDGGGameInstance::CreateMySession(FString roomName, int playerCount)
{
	if (sessionInterface)
	{
		FOnlineSessionSettings sessionSettings;
		sessionSettings.bAllowInvites = false;
		sessionSettings.bAllowJoinInProgress = true;
		sessionSettings.bAllowJoinViaPresence = true;
		sessionSettings.bShouldAdvertise = true;
		sessionSettings.bIsDedicated = false;
		sessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
		sessionSettings.NumPublicConnections = playerCount;
		sessionSettings.Set(FName("KEY_RoomName"), roomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		sessionInterface->CreateSession(0, *roomName, sessionSettings);

		UE_LOG(LogTemp, Warning, TEXT("Create Session Try"));
	}
}

void UBDGGGameInstance::FindMySession()
{
	sessionSearch = MakeShareable(new FOnlineSessionSearch);
	sessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	sessionSearch->MaxSearchResults = 30;
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
}

void UBDGGGameInstance::JoinMySession(int sessionIndex)
{
	FOnlineSessionSearchResult selectedSession = sessionSearch->SearchResults[sessionIndex];
	sessionInterface->JoinSession(0, sessionID, selectedSession);
}

