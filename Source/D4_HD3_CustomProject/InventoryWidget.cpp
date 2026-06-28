// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "D4_HD3_CustomProjectCharacter.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Food.h"

void UInventoryWidget::OnButtonWasClicked(UInventoryButtonWidget* Button)
{
	SelectedItemIndex = ButtonList.Find(Button);

	AFood* SelectedItem = Owner->GetItemAtIndex(SelectedItemIndex);
	if(SelectedItem) {
		ItemNameTextBlock->
			  SetText(FText::FromString(SelectedItem->GetName()));
		ItemDescriptionTextBlock->
			SetText(FText::FromString(SelectedItem->GetDescription()));
	}
	else
	{
		ItemNameTextBlock->SetText(FText::FromString("No Item Selected"));
		ItemDescriptionTextBlock->SetText(FText::FromString("No Item Selected"));
		SelectedItemIndex = -1;
	}
}

void UInventoryWidget::OnUseButtonClicked()
{
	Owner->UseItem(SelectedItemIndex);
	ItemNameTextBlock->SetText(FText::FromString("No Item Selected"));
	ItemDescriptionTextBlock->SetText(FText::FromString("No Item Selected"));
	SelectedItemIndex = -1;
}

void UInventoryWidget::OnDeleteButtonClicked()
{
	Owner->DeleteItemAtIndex(SelectedItemIndex);
	ItemNameTextBlock->SetText(FText::FromString("No Item Selected"));
	ItemDescriptionTextBlock->SetText(FText::FromString("No Item Selected"));
	SelectedItemIndex = -1;
}

FReply UInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FKey PressedKey = InKeyEvent.GetKey();
	
	if (PressedKey == EKeys::I)
	{
		if (Owner)
		{
			Owner->ToggleInventory();
			return FReply::Handled();
		}
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UInventoryWidget::RefreshInventory(TArray<AFood*> Items)
{
	for(UInventoryButtonWidget* Btn : ButtonList)
	{
		Btn->UpdateItemIcon(*ImageMap.Find("None"));
	}
	for(int i = 0; i < Items.Num(); i++)
	{
		if(i > ButtonList.Num() - 1)
		{
			return;
		}
		if (UTexture2D* Image = *ImageMap.Find(Items[i]->GetName()))
		{
			ButtonList[i]->UpdateItemIcon(Image);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Item %s does not have image mapping for its name in inventory"), *Items[i]->GetName());
		}
	}
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(ButtonList.Num() > 0) {
		return;
	}

	for(int32 Y = 0; Y < NumButtonHeight; Y++)
	{
		for(int32 X = 0; X < NumButtonWidth; X++)
		{
			FString WidgetName = FString::Printf(TEXT("Button_X%d_Y%x"), X, Y);

			if (BPButtonWidget)
			{
				UInventoryButtonWidget* Button = WidgetTree->ConstructWidget<UInventoryButtonWidget>(BPButtonWidget, FName(WidgetName));

				ButtonList.Add(Button);
				Button->OnClickedDelegate.AddDynamic(this, &UInventoryWidget::OnButtonWasClicked);
				InventoryGrid->AddChildToGrid(Button, Y, X);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("BPButtonWidget is null"));
			}
		}
	}
	
	ItemNameTextBlock->SetText(FText::FromString("No Item Selected"));
	ItemDescriptionTextBlock->SetText(FText::FromString("No Item Selected"));

	UseItemButton->OnClicked.AddUniqueDynamic(this, &UInventoryWidget::OnUseButtonClicked);
	DeleteItemButton->OnClicked.AddUniqueDynamic(this, &UInventoryWidget::OnDeleteButtonClicked);
}
