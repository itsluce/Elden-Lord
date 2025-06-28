// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PawnCombatComponent.h"
#include "EldenCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class ELDENLORD_API UEldenCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
public:
	// UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	// AWeapon* GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const;
	//
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWeapon* GetHeroCurrentEquippedWeapon() const;
	
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	float GetHeroCurrentEquipWeaponDamageAtLevel(float InLevel) const;
	
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	float GetHeroCurrentEquipWeaponSpecialHeavyAtLevel(float InLevel) const;
	
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	float GetHeroCurrentEquipWeaponSpecialLightAtLevel(float InLevel) const;

	virtual void OnHitTargetActor(AActor* HitActor) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor) override;
};
