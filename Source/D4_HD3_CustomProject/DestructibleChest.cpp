// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleChest.h"

ADestructibleChest::ADestructibleChest()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	
	ChestTopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chest Top"));
	ChestBottomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chest Bottom"));
	ChestHinge = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Chest Hinge"));
	
	ChestBottomMesh->SetupAttachment(RootComponent);
	ChestTopMesh->SetupAttachment(ChestBottomMesh);
	ChestHinge->SetupAttachment(ChestBottomMesh);
	
	HealthWidgetComponent->SetupAttachment(ChestBottomMesh);
	HealthWidgetComponent->SetRelativeLocation(FVector(0.0f, -25.0f, 100.0f)); 
	
	MaxHealth = 30.0f;
}

void ADestructibleChest::BeginPlay()
{
	Super::BeginPlay();
	
	ChestTopMesh->SetSimulatePhysics(false);
	ChestTopMesh->SetCollisionObjectType(ECC_GameTraceChannel1);
	ChestTopMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, 
	ECR_Ignore);
	ChestTopMesh->SetCollisionResponseToChannel(ECC_WorldStatic, 
	ECR_Ignore);
}

void ADestructibleChest::OnDestructed()
{
	Super::OnDestructed();
	
	ChestTopMesh->SetSimulatePhysics(true);
}

void ADestructibleChest::DealDamage_Implementation(float DamageTaken, AActor* DamagedBy)
{
	Super::DealDamage_Implementation(DamageTaken, DamagedBy);
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 
			10.0f, 
			FColor::Yellow, 
			FString::Printf(TEXT("DealDamage function is called in chest class"))
			);
	}
}


