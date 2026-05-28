// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"

#include "D4_HD3_CustomProjectCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFood::AFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Apple Mesh"));
	RootComponent = MeshComponent;
	
	PickupCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup Collider"));
	PickupCollider->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
	
	PickupCollider->OnComponentBeginOverlap.AddDynamic(this, &AFood::OnOverlap);
	
	PickupCollider->OnComponentEndOverlap.AddDynamic(this, &AFood::OnEndOverlap);
}

void AFood::OnOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && !SpawnedUI)
	{
		AddPickupUI_Implementation();
	}
}

void AFood::OnEndOverlap(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && SpawnedUI)
	{
		SpawnedUI->RemoveFromParent();
		SpawnedUI = nullptr;
	}
}

void AFood::EatenBy_Implementation(ACharacter* Character)
{
	IEdible::EatenBy_Implementation(Character);
	
	Cast<AD4_HD3_CustomProjectCharacter>(Character)->GainExperience(ExperienceAmount);
}

void AFood::AddPickupUI_Implementation()
{
	IEdible::AddPickupUI_Implementation();
	
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

// Called every frame
void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

