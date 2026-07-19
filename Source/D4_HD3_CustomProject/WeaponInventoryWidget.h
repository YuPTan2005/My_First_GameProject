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
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UPROPERTY(meta=(BindWidget))
	UBorder* BackgroundCloseBorder;
	UFUNCTION()
	FEventReply OnBackgroundButtonClicked(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	
	void UnequipOwnerWeapon(int8 WeaponIndex) const;
	
};
