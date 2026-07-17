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

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(RootComponent);
	
	PickupCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup Collider"));
	PickupCollider->SetupAttachment(RootComponent);

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
	
	if (Item)
	{
		Item->Destroy();
	}
	
	Destroy();
}

void APickupItem::OnOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UItemCollector>() && OtherActor != this && !SpawnedUI)
	{
		if (AD4_HD3_CustomProjectCharacter* Player = Cast<AD4_HD3_CustomProjectCharacter>(OtherActor))
		{
			if (Player->GetHasBackpack())
			{
				AddPickupUI();
			}
		}
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
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (PickupUIClass)
		{
			SpawnedUI = CreateWidget<UPickupUI>(GetGameInstance(), PickupUIClass);
			
			FVector UITextOffset = PC->PlayerCameraManager->GetActorRightVector();

			UITextOffset *= 100;

			UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), 
			MeshComponent->GetComponentLocation() + UITextOffset, SpawnedUI->CurrentLocation);

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

AActor* APickupItem::GetItem() const
{
	return Item;
}

void APickupItem::SetItem(AActor* NewItem)
{
	Item = NewItem;
}

AActor* APickupItem::GetPickerActor() const
{
	return PickerActor;
}

void APickupItem::SetPickerActor(AActor* NewActor)
{
	PickerActor = NewActor;
}

void APickupItem::PickedUp()
{
	Destroy();
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

