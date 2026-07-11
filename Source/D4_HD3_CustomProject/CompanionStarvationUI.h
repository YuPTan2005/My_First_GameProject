// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CompanionStarvationUI.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UCompanionStarvationUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Pulse;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* StarvationWarningTextBlock;
	
	FString StarvationText = "Companion starving";
	
	virtual void NativeConstruct() override;

public:
	void TriggerAnimation();
	void EndAnimation();
	
};
