// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInventoryActorComponent.h"

#include "D4_HD3_CustomProjectCharacter.h"

UWeaponInventoryActorComponent::UWeaponInventoryActorComponent()
{
	InventorySize = 2;
}

bool UWeaponInventoryActorComponent::UseItemAtIndex(int32 Index, AActor* Character)
{
	if (AD4_HD3_CustomProjectCharacter* Player = Cast<AD4_HD3_CustomProjectCharacter>(Character))
	{
		if (AWeapon* Weapon = Cast<AWeapon>(InventoryItems[Index]))
		{
			Player->AttachWeaponToSocket(Weapon, Weapon->GetSocketName());
			return true;
		}
	}
	
	return false;
}
