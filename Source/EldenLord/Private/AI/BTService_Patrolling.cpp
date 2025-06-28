// luce copyright

#include "AI/BTService_Patrolling.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_Patrolling::UBTService_Patrolling()
{
	NodeName = "Patrol Service";
	WaitTimer = 0.0f;
	bIsWaiting = false;
	bPatrolPointsInitialized = false;
	CurrentTargetPoint = FVector::ZeroVector;
	LastReachedPoint = FVector::ZeroVector;
	SpawnLocation = FVector::ZeroVector;
}

void UBTService_Patrolling::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwningPawn = AIOwner->GetPawn();
	if (!IsValid(OwningPawn))
	{
		return;
	}

	// Set patrol walk speed
	if (ACharacter* Character = Cast<ACharacter>(OwningPawn))
	{
		if (UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement())
		{
			MovementComp->MaxWalkSpeed = 150.0f;
		}
	}

	// Initialize patrol points if not done yet
	if (!bPatrolPointsInitialized)
	{
		InitializePatrolPoints(OwningPawn);
		if (AvailablePatrolPoints.Num() > 0)
		{
			SelectRandomPatrolPoint();
		}
		return;
	}

	// Handle waiting at patrol point
	if (bIsWaiting)
	{
		WaitTimer += DeltaSeconds;
		UBTFunctionLibrary::SetBlackboardValueAsBool(this, IsWaitingSelector, true);
		
		if (WaitTimer >= WaitTimeAtPoint)
		{
			bIsWaiting = false;
			WaitTimer = 0.0f;
			HandlePatrolPointReached();
			SelectRandomPatrolPoint();
		}
		return;
	}

	// Check if reached current patrol point
	if (HasReachedPatrolPoint(OwningPawn))
	{
		if (!bIsWaiting)
		{
			bIsWaiting = true;
			WaitTimer = 0.0f;
			LastReachedPoint = CurrentTargetPoint;
		}
	}
	else
	{
		UBTFunctionLibrary::SetBlackboardValueAsBool(this, IsWaitingSelector, false);
	}
}

void UBTService_Patrolling::InitializePatrolPoints(APawn* OwningPawn)
{
	if (!IsValid(OwningPawn))
	{
		return;
	}

	// Store the spawn location for distance checks
	SpawnLocation = OwningPawn->GetActorLocation();

	// Find all actors with "PatrolPoint" tag
	TArray<AActor*> PatrolPointActors;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, FName("PatrolPoint"), PatrolPointActors);

	AllPatrolPoints.Empty();
	AvailablePatrolPoints.Empty();

	for (AActor* PatrolActor : PatrolPointActors)
	{
		if (IsValid(PatrolActor))
		{
			FVector PatrolLocation = PatrolActor->GetActorLocation();
			
			// Only add patrol points within the maximum patrol distance
			float DistanceFromSpawn = FVector::Dist(SpawnLocation, PatrolLocation);
			if (DistanceFromSpawn <= MaxPatrolDistance)
			{
				AllPatrolPoints.Add(PatrolLocation);
				AvailablePatrolPoints.Add(PatrolLocation);
			}
		}
	}

	bPatrolPointsInitialized = true;

	// If no patrol points found, use some default points around the NPC
	if (AllPatrolPoints.Num() == 0)
	{
		// Use smaller radius to ensure default points stay within MaxPatrolDistance
		float PatrolRadius = FMath::Min(MaxPatrolDistance * 0.5f, 500.0f);
		
		// Create 4 default patrol points in a square pattern around spawn location
		AllPatrolPoints.Add(SpawnLocation + FVector(PatrolRadius, PatrolRadius, 0.0f));
		AllPatrolPoints.Add(SpawnLocation + FVector(PatrolRadius, -PatrolRadius, 0.0f));
		AllPatrolPoints.Add(SpawnLocation + FVector(-PatrolRadius, -PatrolRadius, 0.0f));
		AllPatrolPoints.Add(SpawnLocation + FVector(-PatrolRadius, PatrolRadius, 0.0f));
		
		AvailablePatrolPoints = AllPatrolPoints;
	}
}

void UBTService_Patrolling::SelectRandomPatrolPoint()
{
	if (AvailablePatrolPoints.Num() == 0)
	{
		return;
	}

	// Select random patrol point from available ones
	int32 RandomIndex = FMath::RandRange(0, AvailablePatrolPoints.Num() - 1);
	CurrentTargetPoint = AvailablePatrolPoints[RandomIndex];

	// Remove selected point from available points
	AvailablePatrolPoints.RemoveAt(RandomIndex);

	// Set the patrol point in blackboard
	UBTFunctionLibrary::SetBlackboardValueAsVector(this, PatrolPointSelector, CurrentTargetPoint);
}

void UBTService_Patrolling::HandlePatrolPointReached()
{
	// Add the last reached point back to available points (if it's not already there)
	if (LastReachedPoint != FVector::ZeroVector && !AvailablePatrolPoints.Contains(LastReachedPoint))
	{
		AvailablePatrolPoints.Add(LastReachedPoint);
	}

	// If all points have been visited, reset available points
	if (AvailablePatrolPoints.Num() == 0)
	{
		AvailablePatrolPoints = AllPatrolPoints;
		
		// Remove current target from available points to avoid immediate revisit
		AvailablePatrolPoints.Remove(CurrentTargetPoint);
	}
}

bool UBTService_Patrolling::HasReachedPatrolPoint(APawn* OwningPawn) const
{
	if (!IsValid(OwningPawn) || CurrentTargetPoint == FVector::ZeroVector)
	{
		return false;
	}

	float DistanceToTarget = FVector::Dist(OwningPawn->GetActorLocation(), CurrentTargetPoint);
	return DistanceToTarget <= ReachDistance;
}