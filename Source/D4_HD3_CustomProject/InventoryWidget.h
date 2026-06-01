// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryButtonWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/GridPanel.h"
#include "Components/TextBlock.h"
#include "InventoryWidget.generated.h"

class UInputAction;
class AFood;
class AD4_HD3_CustomProjectCharacter;

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInventoryButtonWidget> BPButtonWidget;

	UPROPERTY(meta=(BindWidget))
	UGridPanel* InventoryGrid;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemNameTextBlock;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemDescriptionTextBlock;
	UPROPERTY(meta=(BindWidget))
	UButton* UseItemButton;
	UPROPERTY(meta=(BindWidget))
	UButton* DeleteItemButton;

	UFUNCTION()
	void OnButtonWasClicked(UInventoryButtonWidget* Button);

	UFUNCTION()
	void OnUseButtonClicked();

	UFUNCTION()
	void OnDeleteButtonClicked();
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	AD4_HD3_CustomProjectCharacter* Owner;
	
	void RefreshInventory(TArray<AFood*> Items);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TArray<UInventoryButtonWidget*> ButtonList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, UTexture2D*> ImageMap;

	int32 SelectedItemIndex = -1;
	
	int32 NumButtonWidth = 3;
	int32 NumButtonHeight = 3;
	
};
