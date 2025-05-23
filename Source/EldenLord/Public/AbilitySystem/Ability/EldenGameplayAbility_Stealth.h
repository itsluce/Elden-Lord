// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/EldenGameplayAbility.h"
#include "EldenGameplayAbility_Stealth.generated.h"

/**
 * 
 */
UCLASS()
class ELDENLORD_API UEldenGameplayAbility_Stealth : public UEldenGameplayAbility
{
	GENERATED_BODY()
			
protected:
	//~ Begin UGameplayAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	UFUNCTION(BlueprintCallable)
	void CalculateEnemies();

private:
	UPROPERTY(EditDefaultsOnly)
	float BoxTraceDistance = 50.f;

	UPROPERTY(EditDefaultsOnly)
	FVector TraceBoxSize = FVector(100.f);
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<	EObjectTypeQuery> > EnemyTraceChannel;

	UPROPERTY(EditDefaultsOnly)
	bool bDrawDebugShape = false;

	UPROPERTY()
	TArray<AEnemy*> CollectedEnemy;
};
