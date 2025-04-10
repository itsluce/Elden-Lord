// luce copyright


#include "AbilitySystem/Ability/EldenGameplayAbility.h"
#include "AbilitySystem/EldenAbilitySystemComponent.h"
#include "Components/PawnCombatComponent.h"
#include "Character/EldenLordCharacter.h"
#include "Player/EldenController.h"


void UEldenGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == EEldenAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UEldenGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AbilityActivationPolicy == EEldenAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UPawnCombatComponent* UEldenGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{	
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UEldenAbilitySystemComponent* UEldenGameplayAbility::GetWarriorAbilitySystemComponentFromActorInfo() const
{
	return Cast<UEldenAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

AEldenLordCharacter* UEldenGameplayAbility::GetHeroCharacterFromActorInfo()
{
	if (!CachedWarriorHeroCharacter.IsValid())
	{
		CachedWarriorHeroCharacter = Cast<AEldenLordCharacter>(CurrentActorInfo->AvatarActor);
	}
   
	return CachedWarriorHeroCharacter.IsValid()? CachedWarriorHeroCharacter.Get() : nullptr;
}

AEldenController* UEldenGameplayAbility::GetHeroControllerFromActorInfo()
{
	if (!CachedWarriorHeroController.IsValid())
	{
		CachedWarriorHeroController = Cast<AEldenController>(CurrentActorInfo->PlayerController);
	}

	return CachedWarriorHeroController.IsValid()? CachedWarriorHeroController.Get() : nullptr;
}

UEldenCombatComponent* UEldenGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	return GetHeroCharacterFromActorInfo()->GetEldenCombatComponent();
}
