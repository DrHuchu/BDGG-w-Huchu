// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"

#include "BDGGGameInstance.h"
#include "GameModeWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "BDGGPlayerController.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	gi = Cast<UBDGGGameInstance>(GetGameInstance());

	lobbyNameArray = { text_LobbyName1, text_LobbyName2, text_LobbyName3, text_LobbyName4 };

	btn_GameStart->OnClicked.AddDynamic(this, &ULobbyWidget::LobbyGameStart);

	gi->GetFirstLocalPlayerController()->SetShowMouseCursor(true);
	//GetOwningPlayerState()->SetPlayerName(gi->sessionID.ToString());
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ULobbyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	RefreshLobbyName();
}

void ULobbyWidget::LobbyGameStart()
{
	auto pc = Cast<ABDGGPlayerController>(GetOwningPlayerState()->GetPlayerController());
	pc->gameModeWidgetUI->AddToViewport();
	this->RemoveFromParent();
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	GetOwningPlayerState()->GetPlayerController()->SetShowMouseCursor(false);
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
