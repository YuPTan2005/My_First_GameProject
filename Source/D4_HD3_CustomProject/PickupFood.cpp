// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupFood.h"

#include "D4_HD3_CustomProjectCharacter.h"
#include "FoodCollector.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APickupFood::APickupFood()
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
void APickupFood::BeginPlay()
{
	Super::BeginPlay();
	
	PickupCollider->OnComponentBeginOverlap.AddDynamic(this, &APickupFood::OnOverlap);
	PickupCollider->OnComponentEndOverlap.AddDynamic(this, &APickupFood::OnEndOverlap);
}

// Called every frame
void APickupFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupFood::Collected_Implementation(AActor* OtherActor)
{
	SetPickerActor(OtherActor);
	Destroy();
}

void APickupFood::UnCollected_Implementation()
{
}

AActor* APickupFood::GetPickerActor() const
{
	return PickerActor;
}

void APickupFood::SetPickerActor(AActor* NewActor)
{
	PickerActor = NewActor;
}

void APickupFood::OnOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UFoodCollector>() && OtherActor != this && !SpawnedUI)
	{
		AddPickupUI(OtherActor);
		IFoodCollector::Execute_AddCollectibleFood(OtherActor, this);
	}
}

void APickupFood::OnEndOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
							   int32 OtherBodyIndex)
{
	if (OtherActor->Implements<UFoodCollector>() && OtherActor != this && SpawnedUI)
	{
		SpawnedUI->RemoveFromParent();
		SpawnedUI = nullptr;
		IFoodCollector::Execute_RemoveCollectibleFood(OtherActor, this);
	}
}

void APickupFood::AddPickupUI(AActor* Actor)
{
	if (AD4_HD3_CustomProjectCharacter* Player = Cast<AD4_HD3_CustomProjectCharacter>(Actor))
	{
		if (Player->GetHasBackpack())
		{
			if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
			{
				SpawnedUI = Cast<UFoodPickupUI>(CreateWidget(GetGameInstance(), PickupUIClass));
		
				FVector UITextOffset = PC->PlayerCameraManager->GetActorRightVector();
		
				UITextOffset *= 100;
	
				UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), 
				MeshComponent->GetComponentLocation() + UITextOffset, SpawnedUI->CurrentLocation);
	
				SpawnedUI->AddToViewport();
			}
		}
	}
}
