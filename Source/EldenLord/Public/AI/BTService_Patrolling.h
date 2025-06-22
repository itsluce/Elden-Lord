// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_Patrolling.generated.h"

/**
 * 
 */
UCLASS()
class ELDENLORD_API UBTService_Patrolling : public UBTService_BlueprintBase
{
	GENERATED_BODY()
	
public:	UBTService_Patrolling();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Patrol")
	FBlackboardKeySelector PatrolPointSelector;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Patrol")
	FBlackboardKeySelector IsWaitingSelector;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Patrol", meta = (ClampMin = "0.1"))
	float WaitTimeAtPoint = 2.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Patrol", meta = (ClampMin = "50.0"))
	float ReachDistance = 100.0f;

private:
	TArray<FVector> AllPatrolPoints;
	TArray<FVector> AvailablePatrolPoints;
	FVector CurrentTargetPoint;
	FVector LastReachedPoint;
	float WaitTimer;
	bool bIsWaiting;
	bool bPatrolPointsInitialized;

	void InitializePatrolPoints(APawn* OwningPawn);
	void SelectRandomPatrolPoint();
	void HandlePatrolPointReached();
	bool HasReachedPatrolPoint(APawn* OwningPawn) const;
};
