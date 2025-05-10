// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EldenAIController.generated.h"

struct FAIStimulus;
class UAISenseConfig_Sight;
class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class ELDENLORD_API AEldenAIController : public AAIController
{
	GENERATED_BODY()
public:
	AEldenAIController(const FObjectInitializer& ObjectInitializer);
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTree;
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UAIPerceptionComponent* EnemyPerceptionComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UAISenseConfig_Sight* AISenseConfig_Sight;

	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config")
	bool bEnableDetourCrowdAvoidance = true;

	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance",UIMin = "1",UIMax = "4"))
	int32 DetourCrowdAvoidanceQuality = 4;

	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance Config", meta = (EditCondition = "bEnableDetourCrowdAvoidance"))
	float CollisionQueryRange = 600.f;
};
