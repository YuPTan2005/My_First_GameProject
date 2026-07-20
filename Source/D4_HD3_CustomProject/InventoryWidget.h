// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryButtonWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "InventoryWidget.generated.h"

class UInputAction;
class AFood;
class AD4_HD3_CustomProjectCharacter;

/**
 * 
 */
UCLASS(Abstract)
class D4_HD3_CUSTOMPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnButtonWasClicked(UInventoryButtonWidget* Button);

	UPROPERTY()
	AD4_HD3_CustomProjectCharacter* Owner;
	
	void RefreshInventory(TArray<AActor*> Items);
	
	void SetSelectedItemIndex(const int8 NewValue);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInventoryButtonWidget> BPButtonWidget;

	UPROPERTY(meta=(BindWidget))
	UUniformGridPanel* InventoryGrid;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemNameTextBlock;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemDescriptionTextBlock;
	UPROPERTY(meta=(BindWidget))
	UScrollBox* DescriptionContainer;
	
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual void ResetDisplayItem();

	UPROPERTY()
	TArray<UInventoryButtonWidget*> ButtonList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, UTexture2D*> ImageMap;

	int8 SelectedItemIndex = -1;
	
	int8 NumButtonWidth = 3;
	int8 NumButtonHeight = 3;
	
};
