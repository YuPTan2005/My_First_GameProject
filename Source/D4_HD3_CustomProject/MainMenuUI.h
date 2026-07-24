// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuUI.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UMainMenuUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(meta=(BindWidget))
	UButton* PlayButton;
	UFUNCTION()
	void OnPlayButtonClicked();
	
	UPROPERTY(meta=(BindWidget))
	UButton* QuitButton;
	UFUNCTION()
	void OnQuitButtonClicked();
	
};
