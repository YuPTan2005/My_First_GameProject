// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupWeapon.h"
#include "D4_HD3_CustomProjectCharacter.h"


APickupWeapon::APickupWeapon()
{
	CollectItemText = "Press X to pick up";
}

AActor* APickupWeapon::PickedUp()
{
	AActor* WeaponCreated = Super::PickedUp();
	
	if (!IsValid(WeaponCreated))
	{
		WeaponCreated = GetWorld()->SpawnActor<AActor>(
			ItemClass,
			GetActorLocation(),
			GetActorRotation()
			);
	}
	
	Destroy();
	
	return WeaponCreated;
}
