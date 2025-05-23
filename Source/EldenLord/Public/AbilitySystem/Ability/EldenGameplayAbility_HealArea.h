// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/EldenGameplayAbility.h"
#include "EldenGameplayAbility_HealArea.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class ELDENLORD_API UEldenGameplayAbility_HealArea : public UEldenGameplayAbility
{
	GENERATED_BODY()
public:
	UEldenGameplayAbility_HealArea();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "Ability|Effects")
	void SpawnHealImpactEffects();
	
	UFUNCTION(BlueprintCallable)
	void ApplyHealing();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Healing")
    float HealRadius;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Healing")
    TSubclassOf<UGameplayEffect> HealEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Healing|Effects")
	UNiagaraSystem* HealImpactNiagaraSystem; 
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Healing")
    bool bShowDebug = false;

private:
	TArray<TWeakObjectPtr<AActor>> SuccessfullyHealedActors;

};
