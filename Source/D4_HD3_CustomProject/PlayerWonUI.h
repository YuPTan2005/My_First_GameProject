// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "PlayerWonUI.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UPlayerWonUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* UIContainer;
	
	UPROPERTY(meta=(BindWidget))
	UButton* BackToMainMenuButton;
	
	UPROPERTY(meta=(BindWidget))
	UButton* BackgroundButton;
	
	UFUNCTION()
	void BackToMainMenu();
	
public:
	void ShowUIWidget() const;
	
};
