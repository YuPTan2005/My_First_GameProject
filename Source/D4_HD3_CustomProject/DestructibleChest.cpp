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
	
	MaxHealth = 1000.0f;
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

void ADestructibleChest::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	HealthWidgetComponent->SetRelativeLocation(FVector(0.0f, -25.0f, 100.0f));
}

void ADestructibleChest::OnDestructed()
{
	Super::OnDestructed();
	
	ChestTopMesh->SetSimulatePhysics(true);
}

