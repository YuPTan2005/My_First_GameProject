// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Companion.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
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
	
protected:
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
	UTextBlock* PlayerLevelText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerDamageText;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StarvationBar;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CompanionLevelText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CompanionDamageText;
	
	UPROPERTY(meta=(BindWidget))
	UBorder* Weapon1Border;
	UPROPERTY(meta=(BindWidget))
	UImage* Weapon1Image;
	
	UPROPERTY(meta=(BindWidget))
	UBorder* Weapon2Border;
	UPROPERTY(meta=(BindWidget))
	UImage* Weapon2Image;
	
	virtual void NativeConstruct() override;
	
	FLinearColor BorderOriginalColor;
	
public:
	UPROPERTY()
	AD4_HD3_CustomProjectCharacter* Player;
	UPROPERTY()
	ACompanion* Companion;
	
	void UpdateValues() const;
	void UpdatePlayerValues() const;
	void UpdateCompanionValues() const;
	
	void SetCompanion(ACompanion* NewCompanion);
	
	bool SetNewWeaponImage(UTexture2D* NewWeaponImage) const;
	void RemoveWeaponImage(int8 WeaponIndex) const;
	
	void SetWeaponBorderColor(const int8 WeaponIndex, FLinearColor BorderColor) const;
	void ResetWeaponBorderColor(int8 WeaponIndex) const;
	
};
