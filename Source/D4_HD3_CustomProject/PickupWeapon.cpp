// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupWeapon.h"
#include "D4_HD3_CustomProjectCharacter.h"
#include "Weapon.h"


APickupWeapon::APickupWeapon()
{
	CollectItemText = "Press B to pick up";
}

void APickupWeapon::PickedUp()
{
	if (AD4_HD3_CustomProjectCharacter* Player = Cast<AD4_HD3_CustomProjectCharacter>(PickerActor))
	{
		if (Cast<AWeapon>(Item))
		{
			Player->AddWeapon(Item);
			Super::PickedUp();
		}
	}
}
