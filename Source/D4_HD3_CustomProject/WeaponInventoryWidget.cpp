// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInventoryWidget.h"
#include "D4_HD3_CustomProjectCharacter.h"
#include "InventoryItem.h"


UWeaponInventoryWidget::UWeaponInventoryWidget()
{
	NumButtonHeight = 2;
	NumButtonWidth = 1;
}

void UWeaponInventoryWidget::OnButtonWasClicked(UInventoryButtonWidget* Button)
{
	int8 NewSelectedItemIndex = ButtonList.Find(Button);
	
	if (SelectedItemIndex == NewSelectedItemIndex)
	{
		UnequipOwnerWeapon(SelectedItemIndex);
		ResetDisplayItem();
	}
	else
	{
		if(AActor* SelectedItem = Owner->GetWeaponAtIndex(NewSelectedItemIndex)) 
		{
			SelectedItemIndex = NewSelectedItemIndex;
			Owner->UseWeapon(SelectedItemIndex);
			
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
	
	if (PressedKey == EKeys::G)
	{
		if (Owner)
		{
			Owner->ToggleWeaponInventory();
			return FReply::Handled();
		}
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply UWeaponInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FEventReply UWeaponInventoryWidget::OnBackgroundButtonClicked(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	if (Owner)
	{
		Owner->ToggleWeaponInventory();
	}
	
	FEventReply Reply;
	Reply.NativeReply = FReply::Handled();
	return Reply;
}

void UWeaponInventoryWidget::UnequipOwnerWeapon(int8 WeaponIndex) const
{
	Owner->UnuseWeapon(WeaponIndex);
	ResetButtonBorderColor(WeaponIndex);
}

void UWeaponInventoryWidget::SetButtonBorderColor(const int8 ButtonIndex, const FLinearColor NewColor) const
{
	if (UWidget* IndexWidget = InventoryGrid->GetChildAt(ButtonIndex))
	{
		if (const UInventoryButtonWidget* ButtonWidget = Cast<UInventoryButtonWidget>(IndexWidget))
		{
			ButtonWidget->SetBorderColor(NewColor);
		}
	}
}

void UWeaponInventoryWidget::ResetButtonBorderColor(const int8 ButtonIndex) const
{
	if (UWidget* IndexWidget = InventoryGrid->GetChildAt(ButtonIndex))
	{
		if (const UInventoryButtonWidget* ButtonWidget = Cast<UInventoryButtonWidget>(IndexWidget))
		{
			ButtonWidget->ResetBorderColor();
		}
	}
}

void UWeaponInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	BackgroundCloseBorder->OnMouseButtonDownEvent.BindUFunction(
		this, 
		GET_FUNCTION_NAME_CHECKED(UWeaponInventoryWidget, OnBackgroundButtonClicked)
		);
}
