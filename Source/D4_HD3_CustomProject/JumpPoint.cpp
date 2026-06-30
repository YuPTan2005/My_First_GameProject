// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpPoint.h"

void AJumpPoint::BeginPlay()
{
	Super::BeginPlay();
	
	CopyLinkPoints();
	OnSmartLinkReached.AddDynamic(this, &AJumpPoint::LinkReached);
}

void AJumpPoint::LinkReached(AActor* Agent, const FVector& Destination)
{
	ACharacter* Enemy = Cast<ACharacter>(Agent);
	if (Enemy)
	{
		FVector LaunchVelocity;
		bool LaunchSucceed = UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), LaunchVelocity,
		   Enemy->GetActorLocation(), Destination, 0.0f, 0.3f);
		
		if (LaunchSucceed)
		{
			Enemy->LaunchCharacter(LaunchVelocity, true, true);
		}
	}
}

void AJumpPoint::CopyLinkPoints()
{
	if (PointLinks.Num() && GetSmartLinkComp())
	{
		GetSmartLinkComp()->Modify();
		GetSmartLinkComp()->SetLinkData(PointLinks[0].Left, PointLinks[0].Right, PointLinks[0].Direction);
	}
}
