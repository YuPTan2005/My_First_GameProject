// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupFood.h"

#include "D4_HD3_CustomProjectCharacter.h"
#include "ItemCollector.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APickupFood::APickupFood()
{
}

// Called when the game starts or when spawned
void APickupFood::BeginPlay()
{
	Super::BeginPlay();
}

void APickupFood::OnOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AD4_HD3_CustomProjectCharacter* Player = Cast<AD4_HD3_CustomProjectCharacter>(OtherActor))
	{
		if (Player->GetHasBackpack())
		{
			Super::OnOverlap(OverlapComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
		}
		else if (!SpawnedUI)
		{
			AddEatingUI();
		}
	}
}

void APickupFood::AddEatingUI()
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
			SpawnedUI->SetDisplayText(EatFoodText);
			SpawnedUI->AddToViewport();
		}
	}
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
