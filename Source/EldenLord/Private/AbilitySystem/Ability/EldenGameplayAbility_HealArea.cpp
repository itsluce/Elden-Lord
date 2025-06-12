// luce copyright


#include "AbilitySystem/Ability/EldenGameplayAbility_HealArea.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EldenDbug.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/EldenAbilitySystemLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Constructor for the UEldenGameplayAbility_HealArea class. Initializes default values for properties.
UEldenGameplayAbility_HealArea::UEldenGameplayAbility_HealArea()
{
    HealImpactNiagaraSystem = nullptr;
    HealRadius = 600.0f;
}

// Called when the ability is activated. This function is responsible for initiating the ability's logic.
void UEldenGameplayAbility_HealArea::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

// Called when the ability ends, either through completion, cancellation, or interruption.
void UEldenGameplayAbility_HealArea::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

// Spawns Niagara particle effects at the location of each actor that was successfully healed.
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
    UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
    SuccessfullyHealedActors.Empty();

    TArray<AActor*> OverlappingActors;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(InstigatorActor); // Ignore the instigator itself

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)); // Only consider Pawns

    // Perform a sphere overlap to find actors within the HealRadius
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

        // Check if the target actor is friendly to the instigator
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
                    // Apply the gameplay effect spec to the target's ability system component
                    FActiveGameplayEffectHandle AppliedEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(
                        *SpecHandle.Data.Get(), TargetASC);

                    if (AppliedEffectHandle.WasSuccessfullyApplied())
                    {
                        HealedCount++;
                        SuccessfullyHealedActors.AddUnique(TargetActor); // Record successfully healed actors
                    }
                }
            }
        }
    }
}