// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/EldenDamageGameplayAbility.h"
#include "EldenMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class ELDENLORD_API UEldenMeleeAttack : public UEldenDamageGameplayAbility,public ICombatInterface
{
	GENERATED_BODY()
public:
	
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
};
