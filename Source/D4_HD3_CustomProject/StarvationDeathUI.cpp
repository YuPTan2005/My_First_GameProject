// Fill out your copyright notice in the Description page of Project Settings.


#include "StarvationDeathUI.h"


void UStarvationDeathUI::TriggerAnimation()
{
	if (Pulse)
	{
		PlayAnimation(Pulse, 0, 0);
	}
}

void UStarvationDeathUI::EndAnimation()
{
	if (Pulse)
	{
		StopAnimation(Pulse);
	}
}
