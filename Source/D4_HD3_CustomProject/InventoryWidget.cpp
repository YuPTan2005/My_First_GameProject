// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "D4_HD3_CustomProjectCharacter.h"
#include "InventoryItem.h"
#include "Blueprint/WidgetTree.h"

void UInventoryWidget::OnButtonWasClicked(UInventoryButtonWidget* Button)
{
	SelectedItemIndex = ButtonList.Find(Button);
	
	if(AActor* SelectedItem = Owner->GetItemAtIndex(SelectedItemIndex)) 
	{
		if (SelectedItem->Implements<UInventoryItem>())
		{
			ItemNameTextBlock->
				 SetText(FText::FromString(IInventoryItem::Execute_GetName(SelectedItem)));
			ItemDescriptionTextBlock->
				SetText(FText::FromString(IInventoryItem::Execute_GetDescription(SelectedItem)));
		}
	}
	else
	{
		ResetDisplayItem();
	}
}

FReply UInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FKey PressedKey = InKeyEvent.GetKey();
	
	if (PressedKey == EKeys::V)
	{
		if (Owner)
		{
			Owner->ToggleInventory();
			return FReply::Handled();
		}
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UInventoryWidget::RefreshInventory(TArray<AActor*> Items)
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
		
		if (Items[i]->Implements<UInventoryItem>() && !IInventoryItem::Execute_GetName(Items[i]).IsEmpty())
		{
			if (UTexture2D* Image = *ImageMap.Find(IInventoryItem::Execute_GetName(Items[i])))
			{
				ButtonList[i]->UpdateItemIcon(Image);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Item %s does not have image mapping for its name in inventory"), *Items[i]->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Item in inventory widget does not implement InventoryItem interface"));
		}
	}
	
	ResetDisplayItem();
}

void UInventoryWidget::SetSelectedItemIndex(const int8 NewValue)
{
	SelectedItemIndex = NewValue;
}

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	SetIsFocusable(true);
	
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
				InventoryGrid->AddChildToUniformGrid(Button, Y, X);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("BPButtonWidget is null"));
			}
		}
	}
	
	ItemNameTextBlock->SetText(FText::FromString("No Item Selected"));
	ItemDescriptionTextBlock->SetText(FText::FromString("No Item Selected"));
	ItemDescriptionTextBlock->SetAutoWrapText(true);
	
	DescriptionContainer->SetOrientation(Orient_Vertical);
	DescriptionContainer->SetScrollBarVisibility(ESlateVisibility::Visible);
	DescriptionContainer->SetScrollbarThickness(FVector2D(12.0f, 12.0f));
	DescriptionContainer->SetConsumeMouseWheel(EConsumeMouseWheel::WhenScrollingPossible);
}

void UInventoryWidget::ResetDisplayItem()
{
	ItemNameTextBlock->SetText(FText::FromString("No Item Selected"));
	ItemDescriptionTextBlock->SetText(FText::FromString("No Item Selected"));
	SelectedItemIndex = -1;
}
