// Fill out your copyright notice in the Description page of Project Settings.


#include "StarvationUI.h"


void UStarvationUI::TriggerAnimation()
{
	if (Pulse)
	{
		PlayAnimation(Pulse, 0, 0);
	}
}

void UStarvationUI::EndAnimation()
{
	if (Pulse)
	{
		StopAnimation(Pulse);
	}
}
