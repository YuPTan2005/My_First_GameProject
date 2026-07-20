// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowFallingActorComponent.h"


void USlowFallingActorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(
		FallTimerHandle, 
		this, 
		&USlowFallingActorComponent::FallingAndGroundCheck, 
		0.02f, 
		true
	);
}

void USlowFallingActorComponent::FallingAndGroundCheck()
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(Owner->GetRootComponent());
	if (!RootComp) return;
	
	FVector MeshCenter = RootComp->Bounds.Origin;
	float HalfHeight = RootComp->Bounds.BoxExtent.Z;

	FVector BottomLoc = MeshCenter - FVector(0.0f, 0.0f, HalfHeight);
	FVector EndLoc = BottomLoc - FVector(0.0f, 0.0f, AcceptableGroundRadius);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult, 
		BottomLoc, 
		EndLoc, 
		ECC_Visibility, 
		QueryParams
	);

	if (bHit)
	{
		if (RootComp->IsSimulatingPhysics())
		{
			FVector CurrentVelocity = RootComp->GetPhysicsLinearVelocity();
			CurrentVelocity.Z = 0.0f; 
			RootComp->SetPhysicsLinearVelocity(CurrentVelocity);
		}
		
		GetWorld()->GetTimerManager().ClearTimer(FallTimerHandle);
		return;
	}
	
	if (RootComp->IsSimulatingPhysics())
	{
		FVector CurrentVelocity = RootComp->GetPhysicsLinearVelocity();
		if (CurrentVelocity.Z < -FallSpeed)
		{
			CurrentVelocity.Z = -FallSpeed;
			RootComp->SetPhysicsLinearVelocity(CurrentVelocity);
		}
	}
}
