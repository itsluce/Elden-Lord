// luce copyright


#include "AbilitySystem/Ability/EldenMeleeAttack.h"
#include "Character/BaseCharacter.h"
#include "MotionWarpingComponent.h"
#include "AbilitySystem/Ability/EldenTargetLock.h"
#include "Interface/CombatInterface.h"
#include "Interface/EnemyInterface.h"
#include "Components/PawnCombatComponent.h"
#include "Enemy/Enemy.h"
#include "AI/EldenAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EldenDbug.h"

UEldenMeleeAttack::UEldenMeleeAttack()
{
	bUseStamina = true;
	StaminaCost = 10.0f;
}

void UEldenMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (bEnableMotionWarping)
		{
			SetupMotionWarping(Character);
			
			// Start continuous updates if enabled
			if (bContinuousUpdate && UpdateFrequency > 0.0f)
			{
				GetWorld()->GetTimerManager().SetTimer(
					MotionWarpUpdateTimer,
					this,
					&UEldenMeleeAttack::UpdateMotionWarping,
					UpdateFrequency,
					true // Loop
				);
			}
		}
	}
}

void UEldenMeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Clear the motion warp update timer
	if (GetWorld() && MotionWarpUpdateTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(MotionWarpUpdateTimer);
	}
	
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (UMotionWarpingComponent* MotionWarping = Character->MotionWarpingComponent)
		{
			MotionWarping->RemoveWarpTarget(MotionWarpTargetName);
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEldenMeleeAttack::SetupMotionWarping(ABaseCharacter* Character)
{
	if (!Character || !Character->MotionWarpingComponent)
	{
		return;
	}

	AActor* Target = GetCurrentTarget();
	if (!Target)
	{
		return;
	}

	FVector TargetLocation = Target->GetActorLocation();
	FVector CharacterLocation = Character->GetActorLocation();
	float DistanceToTarget = FVector::Dist(CharacterLocation, TargetLocation);

	if (ShouldUseMotionWarping(DistanceToTarget, Character))
	{
		SetupAdvancedMotionWarping(Character, Target);
	}
}

AActor* UEldenMeleeAttack::GetCurrentTarget() const
{
	if (const ABaseCharacter* Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo()))
	{
		// For Enemies: Get target from AI Controller or Enemy Interface
		if (const AEnemy* Enemy = Cast<AEnemy>(Character))
		{
			// First try: Get target from Enemy Interface
			if (Enemy->Implements<UEnemyInterface>())
			{
				if (AActor* CombatTarget = IEnemyInterface::Execute_GetCombatTarget(Enemy))
				{
					// Check if target is the enemy itself
					if (CombatTarget != Enemy)
					{
						return CombatTarget;
					}
				}
			}
			
			// Second try: Get target from AI Controller's Blackboard
			if (const AEldenAIController* AIController = Cast<AEldenAIController>(Enemy->GetController()))
			{
				if (const UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent())
				{
					// Try different possible blackboard key names
					TArray<FName> PossibleKeys = {
						FName("TargetToFollow"),
						FName("TargetActor"), 
						FName("Player"),
						FName("Target"),
						FName("AttackTarget")
					};
					
					for (const FName& KeyName : PossibleKeys)
					{
						if (AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(KeyName)))
						{
							// Check if target is the enemy itself
							if (TargetActor != Enemy)
							{
								return TargetActor;
							}
						}
					}
				}
			}
		}
		else
		{
			// For Players: Try to get target from target lock system
			if (UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent())
			{
				FGameplayAbilitySpec* TargetLockSpec = ASC->FindAbilitySpecFromClass(UEldenTargetLock::StaticClass());
				if (TargetLockSpec && TargetLockSpec->IsActive())
				{
					// Target lock is active, target should be available
					// For now we'll use trace fallback, but this could be enhanced
				}
			}
		}
		
		// Fallback: Perform a forward trace to find target
		AActor* TraceTarget = GetTargetThroughTrace(Character);
		if (TraceTarget && TraceTarget != Character)
		{
			return TraceTarget;
		}
	}
	
	return nullptr;
}

AActor* UEldenMeleeAttack::GetTargetThroughTrace(const ABaseCharacter* Character) const
{
	if (!Character || !Character->GetWorld())
	{
		return nullptr;
	}

	FVector Start = Character->GetActorLocation();
	FVector Forward = Character->GetActorForwardVector();
	FVector End = Start + (Forward * MaxWarpDistance);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);

	if (Character->GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Pawn,
		QueryParams))
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			// Check if the hit actor is a valid enemy target
			if (HitActor->Implements<UCombatInterface>())
			{
				return HitActor;
			}
		}
	}

	return nullptr;
}

void UEldenMeleeAttack::SetupAdvancedMotionWarping(ABaseCharacter* Character, AActor* Target)
{
	UMotionWarpingComponent* MotionWarping = Character->MotionWarpingComponent;
	if (!MotionWarping)
	{
		return;
	}

	FVector TargetLocation = Target->GetActorLocation();
	FVector CharacterLocation = Character->GetActorLocation();
	FVector Direction = (TargetLocation - CharacterLocation).GetSafeNormal();
	
	// Use different warp distances for enemies vs players
	const bool bIsEnemy = Cast<AEnemy>(Character) != nullptr;
	float CurrentWarpDistance = bIsEnemy ? EnemyWarpDistance : WarpDistance;
	
	// Calculate optimal attack position (stop short of target)
	FVector WarpLocation = TargetLocation - (Direction * CurrentWarpDistance);
	
	// Calculate rotation to face target
	FRotator WarpRotation = UKismetMathLibrary::FindLookAtRotation(WarpLocation, TargetLocation);
	
	// For enemies, make rotation faster by reducing pitch variation
	if (bIsEnemy)
	{
		WarpRotation.Pitch = FMath::Clamp(WarpRotation.Pitch, -EnemyMaxPitchAngle, EnemyMaxPitchAngle);
	}
	
	// Ensure the warp location is on the ground
	FHitResult HitResult;
	FVector StartTrace = WarpLocation + FVector(0, 0, 100);
	FVector EndTrace = WarpLocation - FVector(0, 0, 500);
	
	if (Character->GetWorld()->LineTraceSingleByChannel(
		HitResult, 
		StartTrace, 
		EndTrace, 
		ECC_WorldStatic))
	{
		WarpLocation.Z = HitResult.Location.Z;
	}
	
	// Create transform for both position and rotation warping
	FTransform WarpTransform(WarpRotation, WarpLocation);
	
	// Add motion warp target
	MotionWarping->AddOrUpdateWarpTargetFromTransform(
		MotionWarpTargetName,
		WarpTransform
	);
}

bool UEldenMeleeAttack::ShouldUseMotionWarping(float DistanceToTarget, const ABaseCharacter* Character) const
{
	if (!bEnableMotionWarping)
	{
		return false;
	}

	// Use different distance settings for enemies vs players
	if (Cast<AEnemy>(Character))
	{
		return DistanceToTarget >= EnemyMinWarpDistance && 
			   DistanceToTarget <= EnemyMaxWarpDistance;
	}
	else
	{
		return DistanceToTarget >= MinWarpDistance && 
			   DistanceToTarget <= MaxWarpDistance;
	}
}

void UEldenMeleeAttack::UpdateMotionWarping()
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo()))
	{
		// Only update if motion warping component exists and ability is still active
		if (Character->MotionWarpingComponent && IsActive())
		{
			// Check if there's still a warp target active
			if (Character->MotionWarpingComponent->FindWarpTarget(MotionWarpTargetName))
			{
				SetupMotionWarping(Character);
			}
		}
		else
		{
			// Stop updating if conditions are no longer met
			if (GetWorld() && MotionWarpUpdateTimer.IsValid())
			{
				GetWorld()->GetTimerManager().ClearTimer(MotionWarpUpdateTimer);
			}
		}
	}
}

