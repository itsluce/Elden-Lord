// luce copyright


#include "Components/EldenCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EldenDbug.h"
#include "EldenGameplayTags.h"
#include "EldenDbug.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystem/EldenAbilitySystemLibrary.h"
#include "Item/Weapon/Weapon.h"

//
// AWeapon* UEldenCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const
// {   
// 	return Cast<AWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
// }
//
// AWeapon* UEldenCombatComponent::GetHeroCurrentEquippedWeapon() const
// {	
// 	return Cast<AWeapon>(GetCharacterCurrentEquippedWeapon());
// }
//
float UEldenCombatComponent::GetHeroCurrentEquipWeaponDamageAtLevel(float InLevel) const
{
	return GetHeroCurrentEquippedWeapon()->HeroWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
}


AWeapon* UEldenCombatComponent::GetHeroCurrentEquippedWeapon() const
{	
	return Cast<AWeapon>(GetCharacterCurrentEquippedWeapon());
}

void UEldenCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}
	 
	OverlappedActors.AddUnique(HitActor);

	// Check for blocking before sending melee hit
	bool bIsValidBlock = false;

	const bool bIsTargetBlocking = UEldenAbilitySystemLibrary::NativeDoesActorHaveTag(HitActor, FEldenGameplayTags::Get().Status_Blocking);
	const bool bIsMyAttackUnblockable = false;
	const float BlockStaminaCost = 20.f;

	if (bIsTargetBlocking && !bIsMyAttackUnblockable)
	{
		// Check if target has enough stamina to block
		if (UEldenAbilitySystemLibrary::HasEnoughStamina(HitActor, BlockStaminaCost))
		{
			bIsValidBlock = UEldenAbilitySystemLibrary::IsValidBlock(GetOwningPawn(), HitActor);
		}
		else
		{
			// Cancel blocking ability due to insufficient stamina
			UEldenAbilitySystemLibrary::CancelBlockingAbilityIfInsufficientStamina(HitActor, BlockStaminaCost);
			bIsValidBlock = false;
		}
	}

	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;

	if (bIsValidBlock)
	{
		// Consume stamina for blocking (stamina cost already defined above)
		UEldenAbilitySystemLibrary::ConsumeStamina(HitActor, BlockStaminaCost);
		
		// Send successful block event instead of damage
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitActor,
			FEldenGameplayTags::Get().Event_SuccessfulBlock,
			Data
		);
		// Send knockdown event to attacker (player)
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			FEldenGameplayTags::Get().Event_KnockDown,
			Data
		);
	}
	else
	{
		// Normal melee hit
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			FEldenGameplayTags::Get().Event_MeleeHit,
			Data
		);
	}
}

void UEldenCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		FEldenGameplayTags::Get().Event_HitPause,
		FGameplayEventData()
	);
}
