// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCStatus.h"

void UNPCStatus::UpdateValues()
{
	if (BindingActor && BindingActor->Implements<UNPCInterface>())
	{
		HealthBar->SetPercent(INPCInterface::Execute_GetCurrentHealth(BindingActor) /
			INPCInterface::Execute_GetMaxHealth(BindingActor));
		CurrentHealthText->SetText(FText::FromString(FString::FromInt
			(INPCInterface::Execute_GetCurrentHealth(BindingActor))));
		MaxHealthText->SetText(FText::FromString(FString::FromInt
			(INPCInterface::Execute_GetMaxHealth(BindingActor))));
	}
}
