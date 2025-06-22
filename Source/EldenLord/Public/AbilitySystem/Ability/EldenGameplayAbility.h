// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Enemy/Enemy.h"
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

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UPROPERTY(EditDefaultsOnly, Category = "EldenAbility")
	EEldenAbilityActivationPolicy AbilityActivationPolicy = EEldenAbilityActivationPolicy::OnTriggered;

	// Stamina system properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina")
	bool bUseStamina = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina", meta = (EditCondition = "bUseStamina"))
	float StaminaCost = 0.0f;


	UFUNCTION(BlueprintPure, Category = "Elden|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Elden|Ability")
	UEldenAbilitySystemComponent* GetWarriorAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Elden|Ability")
	AEldenLordCharacter* GetHeroCharacterFromActorInfo();
	
	UFUNCTION(BlueprintPure, Category = "Elden|Ability")
	AEldenController* GetHeroControllerFromActorInfo();
	
	UFUNCTION(BlueprintPure, Category = "Elden|Ability")
	AEnemy* GetEnemyCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Elden|Ability")
	UEldenCombatComponent* GetHeroCombatComponentFromActorInfo();
	
	UFUNCTION(BlueprintPure, Category = "Elden|Ability")
	FGameplayEffectSpecHandle MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass,float InWeaponBaseDamage,FGameplayTag InCurrentAttackTypeTag,int32 InUsedComboCount);

	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor,const FGameplayEffectSpecHandle& InSpecHandle);
	
	UFUNCTION(BlueprintCallable, Category = "Elden|Ability")
	void ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle,const TArray<FHitResult>& InHitResults);

private:
	TWeakObjectPtr<AEldenLordCharacter> CachedWarriorHeroCharacter;
	TWeakObjectPtr<AEldenController> CachedWarriorHeroController;
	
	TWeakObjectPtr<AEnemy> CachedEnemyCharacter;

};
