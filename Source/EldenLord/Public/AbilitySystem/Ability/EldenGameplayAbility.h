// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Interface/CombatInterface.h"
#include "EldenGameplayAbility.generated.h"

class UEldenCombatComponent;
class AEldenController;
class AEldenLordCharacter;
class UEldenAbilitySystemComponent;
class UPawnCombatComponent;

UENUM(BlueprintType)
enum class EEldenAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};

/**
 * 
 */
UCLASS()
class ELDENLORD_API UEldenGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartUpInputTag;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
	UPROPERTY(EditDefaultsOnly, Category = "EldenAbility")
	EEldenAbilityActivationPolicy AbilityActivationPolicy = EEldenAbilityActivationPolicy::OnTriggered;


	UFUNCTION(BlueprintPure, Category = "Elden|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Elden|Ability")
	UEldenAbilitySystemComponent* GetWarriorAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Elden|Ability")
	AEldenLordCharacter* GetHeroCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Elden|Ability")
	AEldenController* GetHeroControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Elden|Ability")
	UEldenCombatComponent* GetHeroCombatComponentFromActorInfo();

private:
	TWeakObjectPtr<AEldenLordCharacter> CachedWarriorHeroCharacter;
	TWeakObjectPtr<AEldenController> CachedWarriorHeroController;
};
