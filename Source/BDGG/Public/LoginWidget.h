// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

/**
 *
 */
UCLASS()
class BDGG_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;


public:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UEditableText* editText_id;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UEditableText* editText_roomName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class USlider* sl_playerCount;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* text_playerCount;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UButton* btn_Start;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UButton* btn_CreateSession;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UWidgetSwitcher* widgetSwitcher;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UButton* btn_goCreate;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UButton* btn_goFind;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UButton* btn_CreateBack;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* sbox_roomList;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UButton* btn_FindBack;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UButton* btn_Refresh;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class USessionSlotWidget>sessionSlot;

private:

	class UBDGGGameInstance* gameInstance;

	UFUNCTION()
		void ClickStart();

	UFUNCTION()
		void CreateServer();

	UFUNCTION()
		void OnMoveSlider(float val);

	UFUNCTION()
		void GoCreate();

	UFUNCTION()
		void GoFind();

	UFUNCTION()
		void GoBack();

	UFUNCTION()
		void RefreshList();

	UFUNCTION()
		void AddNewSlot(FSessionInfo sessionInfo);

	UFUNCTION()
		void RefreshEnabled();
};
