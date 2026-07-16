// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "D4_HD3_CustomProjectCharacter.h"
#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	SightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	SightConfiguration->SightRadius = SightRadius;
	SightConfiguration->LoseSightRadius = LoseSightRadius;
	SightConfiguration->PeripheralVisionAngleDegrees = FieldOfView;
	SightConfiguration->SetMaxAge(SightAge);
	SightConfiguration->DetectionByAffiliation.bDetectEnemies = true;
	SightConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfiguration->DetectionByAffiliation.bDetectNeutrals = true;
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UAIPerceptionComponent* PerceptionComp = GetPerceptionComponent())
	{
		if (SightConfiguration)
		{
			PerceptionComp->ConfigureSense(*SightConfiguration);
			PerceptionComp->SetDominantSense(*SightConfiguration->GetSenseImplementation());
		}
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
	}
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

FRotator AEnemyAIController::GetControlRotation() const
{
	if(GetPawn())
	{
		return FRotator(0, GetPawn()->GetActorRotation().Yaw,0);
	}
	
	return Super::GetControlRotation();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (!InPawn) return;
	
	ControlledCharacter = Cast<AEnemy>(InPawn);
	
	if (UNavigationSystemBase* NavSystemBase = GetWorld()->GetNavigationSystem())
	{
		NavigationSystem = Cast<UNavigationSystemV1>(NavSystemBase);
	}
	UseBlackboard(AIBlackboard, BlackboardComponent);
	RunBehaviorTree(WalkBehaviourTree);
	
	if (ControlledCharacter)
	{
		ControlledCharacter->bIsFlying = false;
		ControlledCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	
	if (BlackboardComponent && ControlledCharacter)
	{
		BlackboardComponent->SetValueAsBool("Attack", false);
		BlackboardComponent->SetValueAsBool("Landing", false);
		BlackboardComponent->SetValueAsBool("FlyLaunch", false);
		BlackboardComponent->SetValueAsFloat("AttackDistance", ControlledCharacter->AttackDistance);
		BlackboardComponent->SetValueAsBool("ChasePlayer", false);
		BlackboardComponent->SetValueAsObject("Target", nullptr);
	}
	
	TargetPlayer = nullptr;
}

void AEnemyAIController::OnLanding()
{
	GetPawn()->SetActorRotation(GetControlRotation());
	
	if (ControlledCharacter)
	{
		ControlledCharacter->bIsFlying = false;
		ControlledCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		ControlledCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		ControlledCharacter->DisplayLandingMontage();
	}
	RunBehaviorTree(WalkBehaviourTree);
	BlackboardComponent->SetValueAsBool("Landing", false);
}

void AEnemyAIController::OnFlying()
{
	if (ControlledCharacter)
	{
		ControlledCharacter->bIsFlying = true;
		ControlledCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		ControlledCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		ControlledCharacter->DisplayFlyLaunchMontage();
	}
	RunBehaviorTree(FlyBehaviourTree);
	BlackboardComponent->SetValueAsBool("FlyLaunch", false);
}

void AEnemyAIController::DecideMovementMode()
{
	// Decide to fly or walk after complete last patrol
	if (ControlledCharacter)
	{
		if (FMath::RandBool()) // Decide to walk
		{
			if (ControlledCharacter->bIsFlying) // Change from flying to walking
			{
				BlackboardComponent->SetValueAsBool("Landing", true);
			}
		}
		else // Decide to fly
		{
			if (!ControlledCharacter->bIsFlying)
			{
				BlackboardComponent->SetValueAsBool("FlyLaunch", true);
			}
		}
	}
}

void AEnemyAIController::GenerateNewRandomLocationLand()
{
	if(NavigationSystem && GetPawn())
	{
		FNavLocation ReturnLocation;
		NavigationSystem->GetRandomReachablePointInRadius
		   (GetPawn()->GetActorLocation(), PatrolDistance,
		   ReturnLocation);
		BlackboardComponent->SetValueAsVector("PatrolDestination",
		   ReturnLocation.Location);
	}
}

void AEnemyAIController::GenerateNewRandomLocationMidAir()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSys)
		{
			FNavLocation RandomNavPoint;
			FVector Origin = ControlledPawn->GetActorLocation();
		
			if (NavSys->GetRandomReachablePointInRadius(Origin, PatrolDistance, RandomNavPoint))
			{
				FVector MidAirLocation = RandomNavPoint.Location;
			
				float HoverHeight = FMath::RandRange(200.0f, MaxPatrolHeight); 
				MidAirLocation.Z += HoverHeight;

				BlackboardComponent->SetValueAsVector("PatrolDestination", MidAirLocation);
			}
		}
	}
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!BlackboardComponent || !ControlledCharacter) return;
	
	if(Actor->Implements<UDamageable>() && IDamageable::Execute_GetTeam(Actor) != EGameTeam::Enemies) 
	{
		// New alive actor coming into perception and currently doesn't have target player or has dead
		if (Stimulus.WasSuccessfullySensed() && !IDamageable::Execute_IsDead(Actor))
		{
			if (!IsValid(TargetPlayer))
			{
				ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = EnemyIncreasedWalkSpeed;
				ControlledCharacter->GetCharacterMovement()->MaxFlySpeed = EnemyIncreasedFlySpeed;
				TargetPlayer = Actor;
				BlackboardComponent->SetValueAsBool("ChasePlayer", true);
				BlackboardComponent->SetValueAsObject("Target", TargetPlayer);
			}
		}
		else
		{
			ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = EnemyOriginalWalkSpeed;
			ControlledCharacter->GetCharacterMovement()->MaxFlySpeed = EnemyOriginalFlySpeed;
			TargetPlayer = nullptr;
			BlackboardComponent->SetValueAsBool("ChasePlayer", false);
			BlackboardComponent->SetValueAsObject("Target", nullptr);
		}
	}
}

void AEnemyAIController::UpdateAttackCheck_Implementation()
{
	if (BlackboardComponent && ControlledCharacter)
	{
		BlackboardComponent->SetValueAsBool("AttackPossible", false);
		ControlledCharacter->bCanAttack = false;
		
		if (IsValid(TargetPlayer) && TargetPlayer->Implements<UDamageable>())
		{
			FVector TargetLocation = TargetPlayer->GetActorLocation();
			float TargetCharacterRadius = TargetPlayer->GetRootComponent()->Bounds.SphereRadius;
		
			FVector CurrentLocation = ControlledCharacter->GetActorLocation();
			float ControlledCharacterRadius = ControlledCharacter->GetRootComponent()->Bounds.SphereRadius;
		
			if (FVector::Dist(TargetLocation, CurrentLocation) - TargetCharacterRadius - ControlledCharacterRadius
					<= ControlledCharacter->AttackDistance 
				&& !IDamageable::Execute_IsDead(TargetPlayer))
			{
				BlackboardComponent->SetValueAsBool("AttackPossible", true);
				ControlledCharacter->bCanAttack = true;
			}
		}
		// Target Player has dead or lost track of
		else
		{
			TargetPlayer = nullptr;
			BlackboardComponent->SetValueAsBool("ChasePlayer", false);
			BlackboardComponent->SetValueAsObject("Target", nullptr);
		}
	}
}

void AEnemyAIController::Attack_Implementation()
{
	if (TargetPlayer && ControlledCharacter)
	{
		FVector TargetLocation = TargetPlayer->GetActorLocation();
		FVector CurrentLocation = ControlledCharacter->GetActorLocation();
	
		FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
		ControlledCharacter->SetActorRotation(LookRotation);
		
		ControlledCharacter->Attack(TargetPlayer);
	}
}
