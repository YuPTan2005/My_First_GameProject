// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupUI.h"

void UPickupUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetPositionInViewport(CurrentLocation);
}

void UPickupUI::SetDisplayText(const FString& NewText) const
{
	DisplayText->SetText(FText::FromString(NewText));
}
