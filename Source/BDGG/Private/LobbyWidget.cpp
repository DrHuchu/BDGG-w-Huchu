// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"

#include <string>

#include "BDGGGameInstance.h"
#include "GameModeWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
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

	if (!GetOwningPlayer()->HasAuthority())
	{
		btn_GameStart->SetVisibility(ESlateVisibility::Hidden);
		StartBG->SetVisibility(ESlateVisibility::Hidden);
		StartHoverFrame->SetVisibility(ESlateVisibility::Hidden);
		StartFrame->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ULobbyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	RefreshLobbyName();
}

void ULobbyWidget::LobbyGameStart()
{
	gi->totalPlayerNum = tempTotalPlayerNum;

	GetOwningPlayerState()->GetPlayerController()->SetShowMouseCursor(false);
	GetWorld()->ServerTravel("/Game/Maps/HuchuMap_DesignTest?Listen");
}

void ULobbyWidget::RefreshLobbyName()
{
	auto tempArray = UGameplayStatics::GetGameState(GetWorld())->PlayerArray;

	tempArray.Sort([](const APlayerState& A, const APlayerState& B)
		{
			return A.GetStartTime() < B.GetStartTime();
		});

	for(int i = 0; i < tempArray.Num(); i++)
	{
		lobbyNameArray[i]->SetVisibility(ESlateVisibility::Visible);
		lobbyNameArray[i]->SetText(FText::FromString(tempArray[i]->GetPlayerName()));
		lobbyPingArray[i]->SetVisibility(ESlateVisibility::Visible);
		if(i == 0)
		{
			lobbyPingArray[0]->SetText(FText::FromString("Server"));
		}
		else
		{
			lobbyPingArray[i]->SetText(FText::FromString((FString::FromInt(tempArray[i]->GetPingInMilliseconds()) + ("ms"))));
		}
	}

	if (tempTotalPlayerNum < tempArray.Num())
	{
		tempTotalPlayerNum = tempArray.Num();
	}
}

