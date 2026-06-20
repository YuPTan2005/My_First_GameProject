// Copyright Epic Games, Inc. All Rights Reserved.

#include "D4_HD3_CustomProjectGameMode.h"

#include "RespawnPosition.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AD4_HD3_CustomProjectGameMode::AD4_HD3_CustomProjectGameMode()
{
	// stub
}

void AD4_HD3_CustomProjectGameMode::RespawnPlayer(ACharacter* Player)
{
	if (!Player)
	{
		return;
	}
	
	if (UWorld* WorldObj = GetWorld())
	{
		if (AActor* RespawnPos = UGameplayStatics::GetActorOfClass(WorldObj, ARespawnPosition::StaticClass()))
		{
			FTransform SpawnTransform = RespawnPos->GetActorTransform();
			FVector SpawnLocation = SpawnTransform.GetLocation();
			FRotator SpawnRotation = SpawnTransform.Rotator();
	
			AActor* SpawnedActor = WorldObj->SpawnActor(
				Player->GetClass(),
				&SpawnLocation,
				&SpawnRotation
			);
	
			if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				if (APawn* SpawnedPawn = Cast<APawn>(SpawnedActor))
				{
					PlayerController->Possess(SpawnedPawn);
				}
			
				PlayerController->SetInputMode(FInputModeGameOnly());
				PlayerController->SetShowMouseCursor(false);
			}
		}
	}
}