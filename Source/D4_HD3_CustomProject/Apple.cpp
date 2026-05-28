// Fill out your copyright notice in the Description page of Project Settings.


#include "Apple.h"

// Sets default values
AApple::AApple()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ExperienceAmount = 10;
}

// Called when the game starts or when spawned
void AApple::BeginPlay()
{
	Super::BeginPlay();
	
	PickupCollider->OnComponentBeginOverlap.AddDynamic(this, &AApple::OnOverlap);
}

// Called every frame
void AApple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
