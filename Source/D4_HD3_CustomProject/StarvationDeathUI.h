// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/WidgetAnimation.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StarvationDeathUI.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UStarvationDeathUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* Pulse;

public:
	void TriggerAnimation();
	void EndAnimation();
	
};
