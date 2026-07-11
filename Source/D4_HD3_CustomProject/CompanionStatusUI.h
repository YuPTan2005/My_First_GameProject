// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "CompanionStatusUI.generated.h"

class ACompanion;
/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UCompanionStatusUI : public UUserWidget
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
	UProgressBar* StarvationBar;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxStarvationText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentStarvationText;
	
public:
	UPROPERTY()
	ACompanion* BindingActor;
	
	void UpdateValues() const;
	
	void SetMaxHealthTextSize(float NewSize) const;
	void SetCurrentHealthTextSize(float NewSize) const;
	
	void SetMaxStarvationTextSize(float NewSize) const;
	void SetCurrentStarvationTextSize(float NewSize) const;
	
};
