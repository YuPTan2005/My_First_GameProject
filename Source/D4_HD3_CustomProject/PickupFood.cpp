// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupFood.h"

#include "D4_HD3_CustomProjectCharacter.h"
#include "Food.h"
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
	if (OtherActor->Implements<UItemCollector>() && OtherActor != this)
	{
		if (AD4_HD3_CustomProjectCharacter* Player = Cast<AD4_HD3_CustomProjectCharacter>(OtherActor))
		{
			if (!Player->GetHasBackpack())
			{
				if (!EatUI && !FeedUI)
				{
					EatUI = CreateUIWidget(EatFoodText, EatFoodTextRightOffset, EatFoodTextVerticalOffset);
					FeedUI = CreateUIWidget(FeedFoodText, FeedFoodTextRightOffset, FeedFoodTextVerticalOffset);
					IItemCollector::Execute_AddCollectibleItem(OtherActor, this);
				}
				return;
			}
		}
			
		Super::OnOverlap(OverlapComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

void APickupFood::OnEndOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (EatUI)
	{
		EatUI->RemoveFromParent();
		EatUI = nullptr;
	}

	if (FeedUI)
	{
		FeedUI->RemoveFromParent();
		FeedUI = nullptr;
	}
	
	Super::OnEndOverlap(OverlapComp, OtherActor, OtherComp, OtherBodyIndex);
}

UPickupUI* APickupFood::CreateUIWidget(const FString& UIText, const float UITextOffsetMultiplier, const FVector& VerticalOffset) const
{
	if (PickupUIClass)
	{
		UPickupUI* NewUI = CreateWidget<UPickupUI>(GetWorld(), PickupUIClass);
		if (NewUI)
		{
			if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
			{
				FVector UITextOffset = PC->PlayerCameraManager->GetActorRightVector() * UITextOffsetMultiplier + VerticalOffset;
            
				FVector2D ScreenPosition;
				UGameplayStatics::ProjectWorldToScreen(PC, MeshComponent->GetComponentLocation() + UITextOffset, ScreenPosition);
				NewUI->CurrentLocation = ScreenPosition;
			}

			NewUI->SetDisplayText(UIText);
			NewUI->AddToViewport();
		}

		return NewUI;
	}
	
	return nullptr;
}

// Called every frame
void APickupFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupFood::Collected_Implementation(AActor* OtherActor)
{
	SetPickerActor(OtherActor);
}

void APickupFood::UnCollected_Implementation()
{
}

AActor* APickupFood::PickedUp()
{
	AActor* FoodCreated = Super::PickedUp();
	
	if (!IsValid(FoodCreated))
	{
		FoodCreated = GetWorld()->SpawnActor<AActor>(
			AFood::StaticClass(),
			GetActorLocation(),
			GetActorRotation()
			);
	}
	
	return FoodCreated;
}
