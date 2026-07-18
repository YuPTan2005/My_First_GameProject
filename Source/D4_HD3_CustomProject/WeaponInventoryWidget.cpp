// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInventoryWidget.h"
#include "D4_HD3_CustomProjectCharacter.h"
#include "InventoryItem.h"


void UWeaponInventoryWidget::OnButtonWasClicked(UInventoryButtonWidget* Button)
{
	int8 NewSelectedItemIndex = ButtonList.Find(Button);
	
	if (SelectedItemIndex == NewSelectedItemIndex)
	{
		ResetDisplayItem();
	}
	else
	{
		if(AActor* SelectedItem = Owner->GetWeaponAtIndex(NewSelectedItemIndex)) 
		{
			SelectedItemIndex = NewSelectedItemIndex;
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
	
}

FReply UWeaponInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FKey PressedKey = InKeyEvent.GetKey();
	
	if (PressedKey == EKeys::O)
	{
		if (Owner)
		{
			Owner->ToggleInventory();
			return FReply::Handled();
		}
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}
