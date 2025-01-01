// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "EldenDamageGameplayAbility.h"
#include "EldenProjectileSpell.generated.h"

class AEldenProjectile;
/**
 * 
 */
UCLASS()
class ELDENLORD_API UEldenProjectileSpell : public UEldenDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="ProjectileSpell")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AEldenProjectile> ProjectileClass;

	
};
