// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "D4_HD3_CustomProjectCharacter.h"
#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	GetPerceptionComponent()->SetDominantSense(*SightConfiguration->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfiguration);
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	ControlledCharacter = Cast<AEnemy>(GetPawn());
	NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	UseBlackboard(AIBlackboard, BlackboardComponent);
	RunBehaviorTree(FlyBehaviourTree);
	if (ControlledCharacter)
	{
		ControlledCharacter->bIsFlying = true;
		ControlledCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	}
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
	BlackboardComponent->SetValueAsBool("Attack", false);
	BlackboardComponent->SetValueAsVector("PatrolDestination", GetPawn()->GetActorLocation());
	BlackboardComponent->SetValueAsBool("Landing", false);
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(TargetPlayer)
	{
		BlackboardComponent->SetValueAsVector("PlayerPosition",
		  TargetPlayer->GetActorLocation());
	}
}

FRotator AEnemyAIController::GetControlRotation() const
{
	if(GetPawn())
	{
		return FRotator(0, GetPawn()->GetActorRotation().Yaw,0);
	}
	
	return Super::GetControlRotation();
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
	TargetPlayer = nullptr;
	BlackboardComponent->SetValueAsBool("ChasePlayer", false);
	if(APawn* SensedPawn = Cast<APawn>(Actor)) {
		if(SensedPawn->IsPlayerControlled()) {
			if (ControlledCharacter && Stimulus.WasSuccessfullySensed())
			{
				ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = EnemyIncreasedWalkSpeed;
				ControlledCharacter->GetCharacterMovement()->MaxFlySpeed = EnemyIncreasedFlySpeed;
				TargetPlayer = SensedPawn;
				BlackboardComponent->SetValueAsBool("ChasePlayer", true);
				BlackboardComponent->SetValueAsVector("PlayerPosition",
				   TargetPlayer->GetActorLocation());
			}
			else
			{
				ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = EnemyOriginalWalkSpeed;
				ControlledCharacter->GetCharacterMovement()->MaxFlySpeed = EnemyOriginalFlySpeed;
				TargetPlayer = nullptr;
				BlackboardComponent->SetValueAsBool("ChasePlayer", false);
			}
		}
	}
}

void AEnemyAIController::UpdateAttackCheck()
{
	BlackboardComponent->SetValueAsBool("AttackPossible", false);
	if (TargetPlayer && GetPawn())
	{
		if (AEnemy* CurrentPawn = Cast<AEnemy>(GetPawn()))
		{
			if (AD4_HD3_CustomProjectCharacter* TargetCharacter = Cast<AD4_HD3_CustomProjectCharacter>(TargetPlayer))
			{
				if (FVector::Dist(TargetPlayer->GetActorLocation(), GetPawn()->GetActorLocation()) <= 250 &&
					!TargetCharacter->bIsDead)
				{
					BlackboardComponent->SetValueAsBool("AttackPossible", true);
					CurrentPawn->bCanAttack = true;
				}
			}
		}
	}
}

void AEnemyAIController::Attack()
{
	if (TargetPlayer && GetPawn())
	{
		if (AEnemy* CurrentPawn = Cast<AEnemy>(GetPawn()))
		{
			CurrentPawn->Attack(TargetPlayer);
		}
	}
}
