// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"

#include "BDGGGameInstance.h"
#include "GameModeWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	gi = Cast<UBDGGGameInstance>(GetGameInstance());

	if (!text_LobbyName1->IsVisible())
	{
		text_LobbyName1->SetVisibility(ESlateVisibility::Visible);
		text_LobbyName1->SetText(FText::FromString(gi->sessionID.ToString()));
	}
	else if (!text_LobbyName2->IsVisible())
	{
		text_LobbyName2->SetVisibility(ESlateVisibility::Visible);
		text_LobbyName2->SetText(FText::FromString(gi->sessionID.ToString()));
	}
	else if (!text_LobbyName3->IsVisible())
	{
		text_LobbyName3->SetVisibility(ESlateVisibility::Visible);
		text_LobbyName3->SetText(FText::FromString(gi->sessionID.ToString()));
	}
	else if (!text_LobbyName4->IsVisible())
	{
		text_LobbyName4->SetVisibility(ESlateVisibility::Visible);
		text_LobbyName4->SetText(FText::FromString(gi->sessionID.ToString()));
	}

	btn_GameStart->OnClicked.AddDynamic(this, &ULobbyWidget::LobbyGameStart);

	UGameplayStatics::SetGamePaused(GetWorld(), true);

	gi->GetFirstLocalPlayerController()->SetShowMouseCursor(true);
}

void ULobbyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	
}

void ULobbyWidget::LobbyGameStart()
{
	gi->gameModeWidgetUI->AddToViewport();
	this->RemoveFromParent();
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	gi->GetFirstLocalPlayerController()->SetShowMouseCursor(false);
}