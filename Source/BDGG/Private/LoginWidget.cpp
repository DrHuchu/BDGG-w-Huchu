// Fill out your copyright notice in the Description page of Project Settings.


#include "BDGG/Public/LoginWidget.h"

#include "SessionSlotWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/Slider.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "BDGG/Public/BDGGGameInstance.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();
	editText_id->SetText(FText::FromString(""));
	editText_roomName->SetText(FText::FromString(""));
	sl_playerCount->SetValue(2.0f);
	text_playerCount->SetText(FText::FromString("2"));

	// 버튼 바인딩
	btn_Start->OnClicked.AddDynamic(this, &ULoginWidget::ClickStart);
	btn_CreateSession->OnClicked.AddDynamic(this, &ULoginWidget::CreateServer);
	sl_playerCount->OnValueChanged.AddDynamic(this, &ULoginWidget::OnMoveSlider);
	btn_goCreate->OnClicked.AddDynamic(this, &ULoginWidget::GoCreate);
	btn_goFind->OnClicked.AddDynamic(this, &ULoginWidget::GoFind);
	btn_Refresh->OnClicked.AddDynamic(this, &ULoginWidget::RefreshList);
	btn_CreateBack->OnClicked.AddDynamic(this, &ULoginWidget::GoBack);
	btn_FindBack->OnClicked.AddDynamic(this, &ULoginWidget::GoBack);

	gameInstance = Cast<UBDGGGameInstance>(GetGameInstance());
	if (gameInstance)
	{
		gameInstance->searchResultDele.AddDynamic(this, &ULoginWidget::AddNewSlot);
		gameInstance->searchFinishedDele.AddDynamic(this, &ULoginWidget::RefreshEnabled);
	}
}

void ULoginWidget::ClickStart()
{
	if (!editText_id->GetText().IsEmpty())
	{
		widgetSwitcher->SetActiveWidgetIndex(1);
		gameInstance->sessionID = FName(*editText_id->GetText().ToString());
	}
}

void ULoginWidget::CreateServer()
{
	gameInstance->CreateMySession(editText_roomName->GetText().ToString(), sl_playerCount->GetValue());
}

void ULoginWidget::OnMoveSlider(float val)
{
	int32 newVal = FMath::RoundHalfFromZero(val);
	text_playerCount->SetText(FText::AsNumber(newVal));
}

void ULoginWidget::GoCreate()
{
	widgetSwitcher->SetActiveWidgetIndex(2);
}

void ULoginWidget::GoFind()
{
	widgetSwitcher->SetActiveWidgetIndex(3);

	RefreshList();
}

void ULoginWidget::GoBack()
{
	widgetSwitcher->SetActiveWidgetIndex(1);
}

void ULoginWidget::RefreshList()
{
	// 방 리스트 초기화
	sbox_roomList->ClearChildren();

	gameInstance->FindMySession();
	btn_Refresh->SetIsEnabled(false);
}

void ULoginWidget::AddNewSlot(FSessionInfo sessionInfo)
{
	// 게임 인스턴스로부터 검색 완료 델리게이트 신호를 받았을 때 실행
	USessionSlotWidget* slotWidget = CreateWidget<USessionSlotWidget>(this, sessionSlot);

	if (slotWidget == nullptr)
	{
		return;
	}

	slotWidget->text_roomName->SetText(FText::FromString(sessionInfo.roomName));
	slotWidget->text_playerInfo->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), sessionInfo.currentPlayers, sessionInfo.maxPlayers)));
	slotWidget->text_ping->SetText(FText::FromString(FString::Printf(TEXT("%d ms"), sessionInfo.ping)));
	slotWidget->slotIdx = sessionInfo.idx;
	sbox_roomList->AddChild(slotWidget);
}

void ULoginWidget::RefreshEnabled()
{
	btn_Refresh->SetIsEnabled(true);
}