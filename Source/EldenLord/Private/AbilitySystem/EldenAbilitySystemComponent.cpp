// luce copyright


#include "AbilitySystem/EldenAbilitySystemComponent.h"

#include "EldenGameplayTags.h"
#include "AbilitySystem/Ability/EldenGameplayAbility.h"

void UEldenAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UEldenAbilitySystemComponent::EffectApplied);
}

void UEldenAbilitySystemComponent::AddCharacterAttributes(const TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartUpAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UEldenGameplayAbility* EldenAbility = Cast<UEldenGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(EldenAbility->StartUpInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UEldenAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if (InputTag.MatchesTag(FEldenGameplayTags::Get().InputTag_Toggleable))
			{
				if (AbilitySpec.IsActive())
				{
					AbilitySpecInputPressed(AbilitySpec);
					CancelAbilityHandle(AbilitySpec.Handle);
				}
				else
				{
					AbilitySpecInputPressed(AbilitySpec);
					TryActivateAbility(AbilitySpec.Handle);
				}
			}
			else
			{
				AbilitySpecInputPressed(AbilitySpec);
				if (!AbilitySpec.IsActive())
				{
					TryActivateAbility(AbilitySpec.Handle);
				}
			}
		}
	}
}

void UEldenAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
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
