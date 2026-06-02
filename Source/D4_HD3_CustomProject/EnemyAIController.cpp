// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "Enemy.h"

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
	
	NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	UseBlackboard(AIBlackboard, BlackboardComponent);
	RunBehaviorTree(BehaviourTree);
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnSensesUpdated);
	BlackboardComponent->SetValueAsBool("Attack", false);
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

void AEnemyAIController::GenerateNewRandomLocation()
{
	if(NavigationSystem && GetPawn())
	{
		FNavLocation ReturnLocation;
		NavigationSystem->GetRandomReachablePointInRadius
		   (GetPawn()->GetActorLocation(), PatrolDistance,
		   ReturnLocation);
		BlackboardComponent->SetValueAsVector("PatrolPoint",
		   ReturnLocation.Location);
	}
}

void AEnemyAIController::OnSensesUpdated(const TArray<AActor*>& UpdatedActors)
{
	TargetPlayer = nullptr;
	BlackboardComponent->SetValueAsBool("ChasePlayer", false);
	for(AActor* Actor : UpdatedActors)
	{
		if(APawn* SensedPawn = Cast<APawn>(Actor)) {
			if(SensedPawn->IsPlayerControlled()) {
				TargetPlayer = SensedPawn;
				BlackboardComponent->SetValueAsBool("ChasePlayer", true);
				BlackboardComponent->SetValueAsVector("PlayerPosition",
				   TargetPlayer->GetActorLocation());
			}
		}
	}
}

void AEnemyAIController::UpdateAttackCheck()
{
	BlackboardComponent->SetValueAsBool("AttackPossible", false);
	if (TargetPlayer && GetPawn())
	{
		if (FVector::Dist(TargetPlayer->GetActorLocation(), GetPawn()->GetActorLocation()) < 500)
		{
			BlackboardComponent->SetValueAsBool("ShootPossible", true);
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
