// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleWall.h"

#include "Field/FieldSystemActor.h"
#include "NPCStatus.h"

// Sets default values
ADestructibleWall::ADestructibleWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollectionComponent"));
	RootComponent = GeometryCollectionComponent;

	HealthWidgetComponent = CreateDefaultSubobject<UNPCStatusComponent>(TEXT("HealthBarComponent"));
	HealthWidgetComponent->SetupAttachment(RootComponent);

	HealthWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f)); 
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComponent->SetDrawSize(FVector2D(150.0f, 40.0f));
}

// Called when the game starts or when spawned
void ADestructibleWall::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
	bIsDead = false;
	
	if (HealthWidgetComponent && HealthWidgetComponent->GetUserWidgetObject())
	{
		if (UNPCStatus* StatusWidget = Cast<UNPCStatus>(HealthWidgetComponent->GetUserWidgetObject()))
		{
			StatusWidget->BindingActor = this;
			StatusWidget->UpdateValues();
		}
	}
}

void ADestructibleWall::UpdateStatus() const
{
	if (HealthWidgetComponent && HealthWidgetComponent->GetUserWidgetObject())
	{
		if (UNPCStatus* StatusWidget = Cast<UNPCStatus>(HealthWidgetComponent->GetUserWidgetObject()))
		{
			StatusWidget->UpdateValues();
		}
	}
}

void ADestructibleWall::TriggerExplosion() const
{
	if (!MasterField) return;
	
	const FName FunctionName = TEXT("CE_Trigger"); 
	if (UFunction* TriggerFunction = MasterField->FindFunction(FunctionName))
	{
		MasterField->ProcessEvent(TriggerFunction, nullptr);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find the Blueprint function!"));
	}
}

// Called every frame
void ADestructibleWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestructibleWall::DealDamage_Implementation(float DamageTaken, AActor* DamagedBy)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageTaken, 0.0f, MaxHealth);
	UpdateStatus();
	
	if (CurrentHealth <= 0 && !bIsDead)
	{
		bIsDead = true;
		TriggerExplosion();
	}
}

bool ADestructibleWall::IsDead_Implementation()
{
	return bIsDead;
}

EGameTeam ADestructibleWall::GetTeam_Implementation()
{
	return EGameTeam::Enemies;
}

