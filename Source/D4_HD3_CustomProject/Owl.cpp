// Fill out your copyright notice in the Description page of Project Settings.


#include "Owl.h"

// Sets default values
AOwl::AOwl()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOwl::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOwl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOwl::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

