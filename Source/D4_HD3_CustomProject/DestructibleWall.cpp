// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleWall.h"

#include "Field/FieldSystemActor.h"

// Sets default values
ADestructibleWall::ADestructibleWall()
{
	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollectionComponent"));
	RootComponent = GeometryCollectionComponent;
}

// Called when the game starts or when spawned
void ADestructibleWall::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADestructibleWall::OnDestructed()
{
	Super::OnDestructed();
	
	if (!MasterField) return;
	
	const FName FunctionName = TEXT("CE_Trigger"); 
	if (UFunction* TriggerFunction = MasterField->FindFunction(FunctionName))
	{
		MasterField->ProcessEvent(TriggerFunction, nullptr);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find the Blueprint function by %s!"), *GetName());
	}
}

// Called every frame
void ADestructibleWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ADestructibleWall::DealDamage_Implementation(float DamageTaken, AActor* DamagedBy)
{
	Super::DealDamage_Implementation(DamageTaken, DamagedBy);
	
	if (CurrentHealth <= 0 && !bIsDead)
	{
		OnDestructed();
	}
}
