// Fill out your copyright notice in the Description page of Project Settings.


#include "CompanionStarvationUI.h"


void UCompanionStarvationUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	StarvationWarningTextBlock->SetText(FText::FromString(StarvationText));
}

void UCompanionStarvationUI::TriggerAnimation()
{
	if (Pulse)
	{
		PlayAnimation(Pulse, 0, 0);
	}
}

void UCompanionStarvationUI::EndAnimation()
{
	if (Pulse)
	{
		StopAnimation(Pulse);
	}
}
