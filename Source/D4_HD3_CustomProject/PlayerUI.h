// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUI.generated.h"

class UProgressBar;
class UTextBlock;
class AD4_HD3_CustomProjectCharacter;
/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	AD4_HD3_CustomProjectCharacter* Player;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ExperienceBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentExperienceText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxExperienceText;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentHealthText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxHealthText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DamageText;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StarvationBar;
	
	void UpdateValues();
};
