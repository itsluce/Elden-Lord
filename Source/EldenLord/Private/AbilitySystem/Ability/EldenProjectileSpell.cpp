// luce copyright


#include "AbilitySystem/Ability/EldenProjectileSpell.h"

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
	if (CombatInterface)
	{
		FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
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
		Projectile->FinishSpawning(SpawnTransform);
	}
}
