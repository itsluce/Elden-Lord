// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/EldenDamageGameplayAbility.h"
#include "EldenMeleeAttack.generated.h"

class ABaseCharacter;
class UMotionWarpingComponent;
class UEldenTargetLock;
class AEnemy;
class AEldenAIController;

/**
 * 
 */
UCLASS()
class ELDENLORD_API UEldenMeleeAttack : public UEldenDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UEldenMeleeAttack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion Warping")
	FName MotionWarpTargetName = TEXT("AttackTarget");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion Warping")
	float WarpDistance = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion Warping")
	float MinWarpDistance = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion Warping")
	float MaxWarpDistance = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion Warping")
	bool bEnableMotionWarping = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion Warping")
	float EnemyWarpDistance = 250.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion Warping")
	float EnemyMinWarpDistance = 150.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion Warping")
	float EnemyMaxWarpDistance = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion Warping")
	float EnemyMaxPitchAngle = 15.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion Warping")
	bool bContinuousUpdate = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Motion Warping")
	float UpdateFrequency = 0.1f;

	UFUNCTION(BlueprintCallable, Category = "Motion Warping")
	void SetupMotionWarping(ABaseCharacter* Character);

	UFUNCTION(BlueprintCallable, Category = "Motion Warping")
	AActor* GetCurrentTarget() const;

private:
	void SetupAdvancedMotionWarping(ABaseCharacter* Character, AActor* Target);
	bool ShouldUseMotionWarping(float DistanceToTarget, const ABaseCharacter* Character) const;
	AActor* GetTargetThroughTrace(const ABaseCharacter* Character) const;
	void UpdateMotionWarping();
	
	FTimerHandle MotionWarpUpdateTimer;
};
