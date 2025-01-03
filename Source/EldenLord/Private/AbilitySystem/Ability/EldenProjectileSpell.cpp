// luce copyright


#include "AbilitySystem/Ability/EldenProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EldenGameplayTags.h"
#include "Actor/EldenProjectile.h"
#include "Interface/CombatInterface.h"

void UEldenProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UEldenProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface && !CombatInterface->Execute_IsDead(GetAvatarActorFromActorInfo()))
	{
		FVector SocketLocation = CombatInterface->Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo());
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

		FTransform SpawnTransform;
		SpawnTransform.SetRotation(Rotation.Quaternion());
		SpawnTransform.SetLocation(SocketLocation);

		AEldenProjectile* Projectile = GetWorld()->SpawnActorDeferred<AEldenProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
			GetAvatarActorFromActorInfo());
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
			DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());

		FEldenGameplayTags GameplayTags = FEldenGameplayTags::Get();
		for (auto& Pair: DamageTypes)
		{
			const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, ScaledDamage);
		}
		
		Projectile->DamageEffectSpecHandle = SpecHandle;

		Projectile->FinishSpawning(SpawnTransform);
	}
}
