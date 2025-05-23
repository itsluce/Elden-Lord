// luce copyright


#include "AbilitySystem/Ability/EldenGameplayAbility_HealArea.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EldenDbug.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/EldenAbilitySystemLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UEldenGameplayAbility_HealArea::UEldenGameplayAbility_HealArea()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnly;
	HealImpactNiagaraSystem = nullptr;
	HealRadius = 600.0f;
}


void UEldenGameplayAbility_HealArea::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
													 const FGameplayAbilityActorInfo* ActorInfo,
													 const FGameplayAbilityActivationInfo ActivationInfo,
													 const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UEldenGameplayAbility_HealArea::EndAbility(const FGameplayAbilitySpecHandle Handle,
												const FGameplayAbilityActorInfo* ActorInfo,
												const FGameplayAbilityActivationInfo ActivationInfo,
												bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEldenGameplayAbility_HealArea::SpawnHealImpactEffects()
{
	AActor* InstigatorActor = GetAvatarActorFromActorInfo(); 
	if (!InstigatorActor) return;

	UWorld* World = InstigatorActor->GetWorld();
	if (!World) return;

	int32 EffectsSpawned = 0;
	for (const TWeakObjectPtr<AActor>& WeakTargetActor : SuccessfullyHealedActors)
	{
		if (AActor* TargetActor = WeakTargetActor.Get())
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				World,
				HealImpactNiagaraSystem,
				TargetActor->GetActorLocation(),
				TargetActor->GetActorRotation() 
			);
			EffectsSpawned++;
		}
	}

	if (bShowDebug)
	{
		FString Msg = FString::Printf(
			TEXT("UEldenGameplayAbility_HealArea: Spawned heal impact effects on %d targets."),
			EffectsSpawned
		);
		Debug::Print(Msg, FColor::Green);
	}
}

void UEldenGameplayAbility_HealArea::ApplyHealing()
{
    AActor* InstigatorActor = GetAvatarActorFromActorInfo();
    if (!InstigatorActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("UEldenGameplayAbility_HealArea::ApplyHealing: InstigatorActor is null."));
        return;
    }

    UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
    if (!SourceASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("UEldenGameplayAbility_HealArea::ApplyHealing: SourceASC is null."));
        return;
    }

    if (!HealEffectClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("UEldenGameplayAbility_HealArea::ApplyHealing: HealEffectClass is not set."));
        return;
    }

    SuccessfullyHealedActors.Empty();

    TArray<AActor*> OverlappingActors;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(InstigatorActor);

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

    UKismetSystemLibrary::SphereOverlapActors(
        InstigatorActor->GetWorld(),
        InstigatorActor->GetActorLocation(),
        HealRadius,
        ObjectTypes,
        nullptr,
        ActorsToIgnore,
        OverlappingActors
    );

    int32 HealedCount = 0;
    for (AActor* TargetActor : OverlappingActors)
    {
        if (!TargetActor) continue;

        if (!UEldenAbilitySystemLibrary::IsNotFriend(InstigatorActor, TargetActor))
        {
            UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
            if (TargetASC)
            {
                FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
                EffectContextHandle.AddSourceObject(InstigatorActor);

                FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
                    HealEffectClass, GetAbilityLevel(), EffectContextHandle);

                if (SpecHandle.IsValid())
                {
                    FActiveGameplayEffectHandle AppliedEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(
                        *SpecHandle.Data.Get(), TargetASC);

                    if (AppliedEffectHandle.WasSuccessfullyApplied())
                    {
                        HealedCount++;
                        SuccessfullyHealedActors.AddUnique(TargetActor); 
                    }
                }
            }
        }
    }

    if (bShowDebug)
    {
        FString Msg = FString::Printf(
            TEXT("UEldenGameplayAbility_HealArea: %s healed %d targets in radius %.2f."),
            *GetNameSafe(InstigatorActor),
            HealedCount,
            HealRadius
        );
        Debug::Print(Msg);
    }
}