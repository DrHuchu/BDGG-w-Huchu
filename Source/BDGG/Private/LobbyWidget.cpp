// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"

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
	for(int i = 0; i < arraySize; i++)
	{
		lobbyNameArray[i]->SetVisibility(ESlateVisibility::Visible);
		lobbyNameArray[i]->SetText(FText::FromString(tempArray[i]->GetPlayerName()));
	}
}

