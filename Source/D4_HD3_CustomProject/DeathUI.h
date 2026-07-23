// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "DeathUI.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UDeathUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* UIContainer;
	
	UPROPERTY(meta=(BindWidget))
	UButton* ReplayButton;
	UFUNCTION()
	void OnReplayButtonClicked();
	
	UPROPERTY(meta=(BindWidget))
	UButton* QuitButton;
	UFUNCTION()
	void OnQuitButtonClicked();
	
	UPROPERTY(meta=(BindWidget))
	UButton* BackgroundButton;

public:
	UPROPERTY()
	ACharacter* Owner;
	
	void ShowUIWidget() const;
	
};
