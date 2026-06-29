// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "NavLinkCustomComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "JumpPoint.generated.h"

/**
 * 
 */
UCLASS()
class D4_HD3_CUSTOMPROJECT_API AJumpPoint : public ANavLinkProxy
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void LinkReached(AActor* Agent, const FVector& Destination);
	
	void CopyLinkPoints();
	
};
