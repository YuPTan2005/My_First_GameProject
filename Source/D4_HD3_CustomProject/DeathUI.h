// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "DeathUI.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UDeathUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	UButton* ReplayButton;
	
	UPROPERTY(meta=(BindWidget))
	UButton* QuitButton;
	
	ACharacter* Owner;
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnReplayButtonClicked();
	
	UFUNCTION()
	void OnQuitButtonClicked();
};
