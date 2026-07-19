// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryButtonWidget.h"
#include "Components/Button.h"

void UInventoryButtonWidget::UpdateItemIcon(UTexture2D* NewIcon)
{
	FButtonStyle NewStyle = MainButton->GetStyle();
	NewStyle.Normal.SetResourceObject(NewIcon);
	NewStyle.Hovered.SetResourceObject(NewIcon);
	NewStyle.Pressed.SetResourceObject(NewIcon);
	NewStyle.Disabled.SetResourceObject(NewIcon);
	MainButton->SetStyle(NewStyle);
}

void UInventoryButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	MainButton->OnClicked.AddUniqueDynamic(this, &UInventoryButtonWidget::OnButtonClicked);
}

void UInventoryButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	BorderOriginalColor = ButtonBorder->GetBrushColor();
}

void UInventoryButtonWidget::OnButtonClicked()
{
	OnClickedDelegate.Broadcast(this);
}

void UInventoryButtonWidget::SetBorderColor(const FLinearColor BorderColor) const
{
	ButtonBorder->SetBrushColor(BorderColor);
}

void UInventoryButtonWidget::ResetBorderColor() const
{
	ButtonBorder->SetBrushColor(BorderOriginalColor);
}
