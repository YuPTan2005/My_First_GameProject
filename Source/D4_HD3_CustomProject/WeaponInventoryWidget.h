// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidget.h"
#include "WeaponInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UWeaponInventoryWidget : public UInventoryWidget
{
	GENERATED_BODY()
	
public:
	UWeaponInventoryWidget();
	
	virtual void OnButtonWasClicked(UInventoryButtonWidget* Button) override;
	
	void SetButtonBorderColor(const int8 ButtonIndex, const FLinearColor NewColor) const;
	void ResetButtonBorderColor(const int8 ButtonIndex) const;
	
protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
	void UnequipOwnerWeapon(int8 WeaponIndex) const;
	
};
