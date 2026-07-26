// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleChest.h"

#include "ChestItem.h"

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
	
	ChestTopInitial = ChestTopMesh->GetComponentTransform();
	ChestTopMesh->SetSimulatePhysics(false);
	ChestTopMesh->SetCollisionObjectType(ECC_GameTraceChannel1);
	ChestTopMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, 
	ECR_Ignore);
	ChestTopMesh->SetCollisionResponseToChannel(ECC_WorldStatic, 
	ECR_Ignore);
	
	SetupChestItems();
}

void ADestructibleChest::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	HealthWidgetComponent->SetRelativeLocation(FVector(0.0f, -25.0f, 100.0f));
}

void ADestructibleChest::Reset()
{
	Super::Reset();
	
	bIsDead = false;
	
	ChestTopMesh->SetSimulatePhysics(false);
	ChestTopMesh->AttachToComponent(ChestBottomMesh, FAttachmentTransformRules::KeepWorldTransform);
	ChestTopMesh->SetWorldTransform(ChestTopInitial);
	
	CurrentHealth = MaxHealth;
	
	for (APickupItem* Item : ChestItems)
	{
		if (IsValid(Item))
		{
			Item->Destroy();
		}
	}
	ChestItems.Empty();
	
	SetupChestItems();
}

void ADestructibleChest::SetupChestItems()
{
	for (TSubclassOf<AActor> PickupItemClass : ChestItemsClasses)
	{
		FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 25);
		if (APickupItem* SpawnedChestItem = GetWorld()->SpawnActor<APickupItem>(PickupItemClass, SpawnLocation, GetActorRotation()))
		{
			ChestItems.Add(SpawnedChestItem);
		}
	}
}

void ADestructibleChest::OnDestructed()
{
	Super::OnDestructed();
	
	ChestTopMesh->SetSimulatePhysics(true);
	
	for (APickupItem* Item : ChestItems)
	{
		if (IsValid(Item) && Item->Implements<UChestItem>())
		{
			IChestItem::Execute_NotifyCollectible(Item);
		}
	}
}

