// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"


// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	
	WeaponMesh->SetVisibility(false);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FString AWeapon::GetSocketName()
{
	return SocketName;
}

FString AWeapon::GetName_Implementation()
{
	return WeaponName;
}

FString AWeapon::GetDescription_Implementation()
{
	return WeaponDescription;
}

UTexture2D* AWeapon::GetWeaponImage() const
{
	return WeaponImage;
}

void AWeapon::SetWeaponMeshVisibility(const bool bNewVisibility) const
{
	if (WeaponMesh)
	{
		WeaponMesh->SetVisibility(bNewVisibility);
	}
}

