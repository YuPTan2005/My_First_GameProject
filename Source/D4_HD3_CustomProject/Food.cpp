// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"

#include "D4_HD3_CustomProjectCharacter.h"

// Sets default values
AFood::AFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ExperienceAmount = 0;
	FoodName = "Default food";
	FoodDescription = "Just a food";
}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFood::EatenBy_Implementation(ACharacter* Character)
{
	IEdible::EatenBy_Implementation(Character);
	
	if (AD4_HD3_CustomProjectCharacter* Player = Cast<AD4_HD3_CustomProjectCharacter>(Character))
	{
		Player->GainExperience(ExperienceAmount);
	}
}

void AFood::SetName(FString Name)
{
	FoodName = Name;
}

void AFood::SetDescription(FString Description)
{
	FoodDescription = Description;
}

FString AFood::GetName()
{
	return FoodName;
}

FString AFood::GetDescription()
{
	return FoodDescription;
}

// Called every frame
void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

