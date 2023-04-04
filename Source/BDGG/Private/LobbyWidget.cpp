// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"

#include <string>

#include "BDGGGameInstance.h"
#include "GameModeWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	gi = Cast<UBDGGGameInstance>(GetGameInstance());

	lobbyNameArray = { text_LobbyName1, text_LobbyName2, text_LobbyName3, text_LobbyName4 };
	lobbyPingArray = { text_LobbyPing1, text_LobbyPing2, text_LobbyPing3, text_LobbyPing4 };

	btn_GameStart->OnClicked.AddDynamic(this, &ULobbyWidget::LobbyGameStart);

	gi->GetFirstLocalPlayerController()->SetShowMouseCursor(true);

	if (!GetOwningPlayerPawn()->HasAuthority())
	{
		btn_GameStart->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ULobbyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	RefreshLobbyName();
}

void ULobbyWidget::LobbyGameStart()
{
	GetOwningPlayerState()->GetPlayerController()->SetShowMouseCursor(false);
	GetWorld()->ServerTravel("/Game/Maps/HuchuMap_DesignTest?Listen");
}

void ULobbyWidget::RefreshLobbyName()
{
	auto tempArray = UGameplayStatics::GetGameState(GetWorld())->PlayerArray;
	int arraySize = tempArray.Num();

	TArray<FLobbyPlayerInfo> lobbyPlayerInfoArr;
	lobbyPlayerInfoArr.Init(tempStruct ,arraySize);
	for (int i = 0; i < arraySize; i++)
	{
		lobbyPlayerInfoArr[i].name = tempArray[i]->GetPlayerName();
		lobbyPlayerInfoArr[i].startTime = tempArray[i]->GetStartTime();
		lobbyPlayerInfoArr[i].ping = tempArray[i]->GetPingInMilliseconds();
	}

	// tempArray를 startTime으로 정렬하여 서버가 누군지 찾는다
	lobbyPlayerInfoArr.Sort([](const FLobbyPlayerInfo& A, const FLobbyPlayerInfo& B)
		{
			return A.startTime < B.startTime;
		});

	for(int i = 0; i < arraySize; i++)
	{
		lobbyNameArray[i]->SetVisibility(ESlateVisibility::Visible);
		lobbyNameArray[i]->SetText(FText::FromString(lobbyPlayerInfoArr[i].name));
		lobbyPingArray[i]->SetVisibility(ESlateVisibility::Visible);
		if(i == 0)
		{
			lobbyPingArray[0]->SetText(FText::FromString("Server"));
		}
		else
		{
			lobbyPingArray[i]->SetText(FText::FromString((FString::FromInt(lobbyPlayerInfoArr[i].ping) + ("ms"))));
		}
	}
}

