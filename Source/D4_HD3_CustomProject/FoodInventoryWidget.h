// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Button.h"
#include "CoreMinimal.h"
#include "InventoryWidget.h"
#include "FoodInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UFoodInventoryWidget : public UInventoryWidget
{
	GENERATED_BODY()
	
public:
	void CancelFeedButton() const;
	
protected:
	UPROPERTY(meta=(BindWidget))
	UButton* UseItemButton;
	UPROPERTY(meta=(BindWidget))
	UButton* FeedCompanionButton;
	UPROPERTY(meta=(BindWidget))
	UButton* DeleteItemButton;
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnUseButtonClicked();
	UFUNCTION()
	void OnFeedButtonClicked();
	UFUNCTION()
	void OnDeleteButtonClicked();
	
};
