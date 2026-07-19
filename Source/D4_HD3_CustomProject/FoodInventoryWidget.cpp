// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodInventoryWidget.h"
#include "D4_HD3_CustomProjectCharacter.h"


void UFoodInventoryWidget::OnUseButtonClicked()
{
	if (SelectedItemIndex != -1)
	{
		Owner->UseItem(SelectedItemIndex);
		ResetDisplayItem();
	}
}

void UFoodInventoryWidget::OnDeleteButtonClicked()
{
	Owner->DeleteItemAtIndex(SelectedItemIndex);
	ResetDisplayItem();
}

void UFoodInventoryWidget::OnFeedButtonClicked()
{
	Owner->FeedItem(SelectedItemIndex);
	ResetDisplayItem();
}

void UFoodInventoryWidget::CancelFeedButton() const
{
	FeedCompanionButton->SetIsEnabled(false);
}

void UFoodInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UseItemButton->OnClicked.AddUniqueDynamic(this, &UFoodInventoryWidget::OnUseButtonClicked);
	FeedCompanionButton->OnClicked.AddUniqueDynamic(this, &UFoodInventoryWidget::OnFeedButtonClicked);
	DeleteItemButton->OnClicked.AddUniqueDynamic(this, &UFoodInventoryWidget::OnDeleteButtonClicked);
}
