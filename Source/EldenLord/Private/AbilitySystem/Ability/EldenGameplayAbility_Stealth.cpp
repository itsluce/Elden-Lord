// luce copyright


#include "AbilitySystem/Ability/EldenGameplayAbility_Stealth.h"
#include "Character/EldenLordCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

void UEldenGameplayAbility_Stealth::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                    const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UEldenGameplayAbility_Stealth::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEldenGameplayAbility_Stealth::CalculateEnemies()
{
	CollectedEnemy.Empty();

	TArray<FHitResult> TraceHits;

	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetHeroCharacterFromActorInfo(),
		GetHeroCharacterFromActorInfo()->GetActorLocation(),
		GetHeroCharacterFromActorInfo()->GetActorLocation() + -GetHeroCharacterFromActorInfo()->GetActorUpVector() * BoxTraceDistance,
		TraceBoxSize / 2.f,
		(-GetHeroCharacterFromActorInfo()->GetActorUpVector()).ToOrientationRotator(),
		EnemyTraceChannel,
		false,
		TArray<AActor*>(),
		bDrawDebugShape? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		TraceHits,
		true
	);

	for (const FHitResult& TraceHit : TraceHits)
	{
		if (AEnemy* FoundEnemy = Cast<AEnemy>(TraceHit.GetActor()))
		{
			CollectedEnemy.AddUnique(FoundEnemy);
		}
	}

	if (CollectedEnemy.IsEmpty())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
	}
}
