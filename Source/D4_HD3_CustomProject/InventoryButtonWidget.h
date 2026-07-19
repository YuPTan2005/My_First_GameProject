// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "InventoryButtonWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUItemButtonOnClicked, class UInventoryButtonWidget*, Button);

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UInventoryButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FUItemButtonOnClicked OnClickedDelegate;
	void UpdateItemIcon(UTexture2D* NewIcon);
	
	void SetBorderColor(const FLinearColor BorderColor) const;
	void ResetBorderColor() const;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta=(BindWidget))
	UBorder* ButtonBorder;
	FLinearColor BorderOriginalColor;
	
	UPROPERTY(meta=(BindWidget))
	UButton* MainButton;

	UFUNCTION()
	void OnButtonClicked();
	
};
