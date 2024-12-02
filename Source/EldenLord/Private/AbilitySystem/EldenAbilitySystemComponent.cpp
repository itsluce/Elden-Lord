// luce copyright


#include "AbilitySystem/EldenAbilitySystemComponent.h"

void UEldenAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UEldenAbilitySystemComponent::EffectApplied);
}

void UEldenAbilitySystemComponent::AddCharacterAttributes(const TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartUpAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		// GiveAbility(AbilitySpec);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UEldenAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                 const FGameplayEffectSpec& EffectSpec,
                                                 FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
	
	
}
