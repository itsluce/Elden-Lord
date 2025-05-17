// luce copyright


#include "Item/PickUp/EldenStonesBase.h"
#include "Character/EldenLordCharacter.h"
#include "AbilitySystem/EldenAbilitySystemComponent.h"
#include "EldenGameplayTags.h"

void AEldenStonesBase::Consume(UEldenAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	check(StoneGameplayEffectClass);

	UGameplayEffect* EffectCDO = StoneGameplayEffectClass->GetDefaultObject<UGameplayEffect>();

	AbilitySystemComponent->ApplyGameplayEffectToSelf(
		EffectCDO,
		ApplyLevel,
		AbilitySystemComponent->MakeEffectContext()
	);

	BP_OnStoneConsumed();
}

void AEldenStonesBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                           bool bFromSweep, const FHitResult& SweepResult)
{
	if (AEldenLordCharacter* Character = Cast<AEldenLordCharacter>(OtherActor))
	{
		if (Character && Character->GetAbilitySystemComponent())
		{
			UEldenAbilitySystemComponent* EldenASC = Cast<UEldenAbilitySystemComponent>(
				Character->GetAbilitySystemComponent());
			if (EldenASC)
			{
				EldenASC->TryActivateAbilityByTag(FEldenGameplayTags::Get().Abilities_PickUp);
			}
		}
	}
}
