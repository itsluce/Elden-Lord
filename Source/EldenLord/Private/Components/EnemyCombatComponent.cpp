// luce copyright


#include "Components/EnemyCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EldenDbug.h"
#include "EldenGameplayTags.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystem/EldenAbilitySystemLibrary.h"
#include "Components/BoxComponent.h"
#include "Enemy/FrostGiant_Enemy.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(HitActor);

	//TODO:: Implement block check
	bool bIsValidBlock = false;

	const bool bIsPlayerBlocking = UEldenAbilitySystemLibrary::NativeDoesActorHaveTag(HitActor,FEldenGameplayTags::Get().Status_Blocking);
	const bool bIsMyAttackUnblockable = false;
	const float BlockStaminaCost = 20.f;

	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		// Check if target has enough stamina to block
		if (UEldenAbilitySystemLibrary::HasEnoughStamina(HitActor, BlockStaminaCost))
		{
			bIsValidBlock = UEldenAbilitySystemLibrary::IsValidBlock(GetOwningPawn(),HitActor);
		}
		else
		{
			// Cancel blocking ability due to insufficient stamina
			UEldenAbilitySystemLibrary::CancelBlockingAbilityIfInsufficientStamina(HitActor, BlockStaminaCost);
			bIsValidBlock = false;
		}
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;

	if (bIsValidBlock)
	{
		// Consume stamina for blocking (stamina cost already defined above)
		UEldenAbilitySystemLibrary::ConsumeStamina(HitActor, BlockStaminaCost);
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitActor,
			FEldenGameplayTags::Get().Event_SuccessfulBlock,
			EventData
		);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			FEldenGameplayTags::Get().Event_KnockDown,
			EventData
		);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitActor,
			FEldenGameplayTags::Get().Event_KnockDown,
			EventData
		);
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			FEldenGameplayTags::Get().Event_MeleeHit,
			EventData
		);
	}
}

void UEnemyCombatComponent::ToggleBodyCollisionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	AFrostGiant_Enemy* OwningEnemyCharacter = GetOwningPawn<AFrostGiant_Enemy>();

	check(OwningEnemyCharacter);

	UBoxComponent* LeftHandCollisionBox = OwningEnemyCharacter->GetWeaponBoxL();
	UBoxComponent* RightHandCollisionBox = OwningEnemyCharacter->GetWeaponBoxR();

	check(LeftHandCollisionBox && RightHandCollisionBox);

	switch (ToggleDamageType)
	{
	case EToggleDamageType::LeftHand:
		LeftHandCollisionBox->SetCollisionEnabled(bShouldEnable? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;

	case EToggleDamageType::RightHand:
		RightHandCollisionBox->SetCollisionEnabled(bShouldEnable? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		break;

	default:
		break;
	}

	if (!bShouldEnable)
	{
		OverlappedActors.Empty();
	}
}
