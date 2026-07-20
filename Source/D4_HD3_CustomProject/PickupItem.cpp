// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"

#include "D4_HD3_CustomProjectCharacter.h"
#include "ItemCollector.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APickupItem::APickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = MeshComponent;
	
	PickupCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup Collider"));
	PickupCollider->SetupAttachment(MeshComponent);

	PickupCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupCollider->SetCollisionObjectType(ECC_WorldDynamic);
	PickupCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickupCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	
	PickupCollider->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::OnOverlap);
	PickupCollider->OnComponentEndOverlap.AddDynamic(this, &APickupItem::OnEndOverlap);
}

void APickupItem::Reset()
{
	Super::Reset();
	
	Destroy();
}

void APickupItem::OnOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UItemCollector>() && OtherActor != this && !SpawnedUI)
	{
		AddPickupUI();
		IItemCollector::Execute_AddCollectibleItem(OtherActor, this);
	}
}

void APickupItem::OnEndOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor->Implements<UItemCollector>() && OtherActor != this && SpawnedUI)
	{
		SpawnedUI->RemoveFromParent();
		SpawnedUI = nullptr;
		IItemCollector::Execute_RemoveCollectibleItem(OtherActor, this);
	}
}

void APickupItem::AddPickupUI()
{
	if (PickupUIClass)
	{
		SpawnedUI = CreateWidget<UPickupUI>(GetGameInstance(), PickupUIClass);
		if (SpawnedUI)
		{
			if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
			{
				FVector UITextOffset = PC->PlayerCameraManager->GetActorRightVector();
				UITextOffset *= 100;

				UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), 
				MeshComponent->GetComponentLocation() + UITextOffset, SpawnedUI->CurrentLocation);
			}
			SpawnedUI->SetDisplayText(CollectItemText);
			SpawnedUI->AddToViewport();
		}
	}
}

bool APickupItem::GetIsNotPickedUp() const
{
	return bIsNotPickedUp;
}

void APickupItem::SetIsNotPickedUp(bool NewValue)
{
	bIsNotPickedUp = NewValue;
}

AActor* APickupItem::GetPickerActor() const
{
	return PickerActor;
}

void APickupItem::SetPickerActor(AActor* NewActor)
{
	PickerActor = NewActor;
}

AActor* APickupItem::PickedUp()
{
	AActor* ItemCreated = nullptr;
	
	if (ItemClass)
	{
		ItemCreated = GetWorld()->SpawnActor<AActor>(
			ItemClass,
			GetActorLocation(),
			GetActorRotation()
			);
	}
	
	return ItemCreated;
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

