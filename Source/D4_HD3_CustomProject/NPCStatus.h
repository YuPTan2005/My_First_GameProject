// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "NPCInterface.h"
#include "NPCStatus.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UNPCStatus : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxHealthText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentHealthText;
	
	UPROPERTY(meta = (BindWidget))
	USizeBox* HealthBarSizeBox;
	
public:
	UPROPERTY()
	AActor* BindingActor;
	
	void UpdateValues();
	
	void SetHealthBarSize(FVector2D NewSize);
	
	void SetMaxHealthTextSize(float NewSize);
	void SetCurrentHealthTextSize(float NewSize);
	
};
