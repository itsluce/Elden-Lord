// luce copyright


#include "Components/EldenCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EldenDbug.h"
#include "EldenGameplayTags.h"
#include "EldenDbug.h"
#include "Abilities/GameplayAbilityTypes.h"

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
// float UEldenCombatComponent::GetHeroCurrentEquippWeaponDamageAtLevel(float InLevel) const
// {
// 	return GetHeroCurrentEquippedWeapon()->HeroWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
// }

void UEldenCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}
	 
	OverlappedActors.AddUnique(HitActor);

	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		FEldenGameplayTags::Get().Event_HitPause,
		Data
	);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		FEldenGameplayTags::Get().Event_MeleeHit,
		FGameplayEventData()
	);
	Debug::Print(TEXT("Start") + HitActor->GetName());
}

void UEldenCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		FEldenGameplayTags::Get().Event_HitPause,
		FGameplayEventData()
	);
	Debug::Print(TEXT("End") + InteractedActor->GetName());

}
