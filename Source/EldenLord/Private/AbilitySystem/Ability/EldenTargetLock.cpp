// luce copyright


#include "AbilitySystem/Ability/EldenTargetLock.h"
#include "EldenDbug.h"
#include "EldenGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/EldenAbilitySystemLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "UI/Widget/EldenUserWidget.h"
#include "UI/Controller/EldenWidgetController.h"
#include "Blueprint/WidgetTree.h"
#include "Character/EldenLordCharacter.h"
#include "Components/SizeBox.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/EldenController.h"

void UEldenTargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	TryLockOnTarget();
	InitTargetLockMovement();
	InitTargetLockMappingContext();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UEldenTargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                  bool bWasCancelled)
{
	ResetTargetLockMovement();
	ResetTargetLockMappingContext();
	CleanUp();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEldenTargetLock::OnTargetLockTick(float DeltaTime)
{
	AEldenLordCharacter* HeroCharacter = Cast<AEldenLordCharacter>(GetHeroCharacterFromActorInfo());
	AEnemy* EnemyCharacter = Cast<AEnemy>(CurrentLockedActor);
	const bool bCharacterDead = HeroCharacter->IsDead_Implementation();
	const bool bEnemyDead = EnemyCharacter->IsDead_Implementation();
	if (!CurrentLockedActor || bCharacterDead ||bEnemyDead)
	{
		CancelTargetLockAbility();
		return;
	}

	SetTargetLockWidgetPosition();

	const bool bShouldOverrideRotation =
		!UEldenAbilitySystemLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(),
		                                                    FEldenGameplayTags::Get().Status_Dodging)
		&&
		!UEldenAbilitySystemLibrary::NativeDoesActorHaveTag(GetHeroCharacterFromActorInfo(),
		                                                    FEldenGameplayTags::Get().Status_Blocking);

	if (bShouldOverrideRotation)
	{
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
			GetHeroCharacterFromActorInfo()->GetActorLocation(),
			CurrentLockedActor->GetActorLocation()
		);

		LookAtRot -= FRotator(TargetLockCameraOffsetDistance, 0.f, 0.f);

		const FRotator CurrentControlRot = GetHeroControllerFromActorInfo()->GetControlRotation();
		const FRotator TargetRot = FMath::RInterpTo(CurrentControlRot, LookAtRot, DeltaTime,
		                                            TargetLockRotationInterpSpeed);

		GetHeroControllerFromActorInfo()->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
		GetHeroCharacterFromActorInfo()->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
	}
}

void UEldenTargetLock::SwitchTarget(const FGameplayTag& InSwitchDirectionTag)
{
	GetAvailableActorsToLock();

	TArray<AActor*> ActorsOnLeft;
	TArray<AActor*> ActorsOnRight;
	AActor* NewTargetToLock = nullptr;

	GetAvailableActorsAroundTarget(ActorsOnLeft, ActorsOnRight);

	if (InSwitchDirectionTag == FEldenGameplayTags::Get().Event_SwitchTarget_Left)
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnLeft);
	}
	else
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnRight);
	}

	if (NewTargetToLock)
	{
		CurrentLockedActor = NewTargetToLock;
	}
}

void UEldenTargetLock::TryLockOnTarget()
{
	GetAvailableActorsToLock();

	if (AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	CurrentLockedActor = GetNearestTargetFromAvailableActors(AvailableActorsToLock);

	if (CurrentLockedActor)
	{
		DrawTargetLockWidget();

		SetTargetLockWidgetPosition();
	}
	else
	{
		CancelTargetLockAbility();
	}
}

void UEldenTargetLock::GetAvailableActorsToLock()
{
	AvailableActorsToLock.Empty();

	TArray<FHitResult> BoxTraceHits;

	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetHeroCharacterFromActorInfo(),
		GetHeroCharacterFromActorInfo()->GetActorLocation(),
		GetHeroCharacterFromActorInfo()->GetActorLocation() + GetHeroCharacterFromActorInfo()->GetActorForwardVector() *
		BoxTraceDistance,
		TraceBoxSize / 2.f,
		GetHeroCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(),
		BoxTraceChannel,
		false,
		TArray<AActor*>(),
		bShowPersistentDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		BoxTraceHits,
		true
	);

	for (const FHitResult& TraceHit : BoxTraceHits)
	{
		if (AActor* HitActor = TraceHit.GetActor())
		{
			if (HitActor != GetHeroCharacterFromActorInfo())
			{
				AvailableActorsToLock.AddUnique(HitActor);
			}
		}
	}
}

AActor* UEldenTargetLock::GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors)
{
	float ClosestDistance = 0.f;
	return UGameplayStatics::FindNearestActor(GetHeroCharacterFromActorInfo()->GetActorLocation(), InAvailableActors,
	                                          ClosestDistance);
}

void UEldenTargetLock::GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft,
                                                      TArray<AActor*>& OutActorsOnRight)
{
	if (!CurrentLockedActor || AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	const FVector PlayerLocation = GetHeroCharacterFromActorInfo()->GetActorLocation();
	const FVector PlayerToCurrentNormalized = (CurrentLockedActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

	for (AActor* AvailableActor : AvailableActorsToLock)
	{
		if (!AvailableActor || AvailableActor == CurrentLockedActor) continue;

		const FVector PlayerToAvailableNormalized = (AvailableActor->GetActorLocation() - PlayerLocation).
			GetSafeNormal();

		const FVector CrossResult = FVector::CrossProduct(PlayerToCurrentNormalized, PlayerToAvailableNormalized);

		if (CrossResult.Z > 0.f)
		{
			OutActorsOnRight.AddUnique(AvailableActor);
		}
		else
		{
			OutActorsOnLeft.AddUnique(AvailableActor);
		}
	}
}

void UEldenTargetLock::DrawTargetLockWidget()
{
	if (!DrawnTargetLockWidget)
	{
		checkf(TargetLockWidgetClass, TEXT("Forgot to assign a valid widget class in Blueprint"));

		DrawnTargetLockWidget = CreateWidget<UEldenUserWidget>(GetHeroControllerFromActorInfo(), TargetLockWidgetClass);

		check(DrawnTargetLockWidget);

		DrawnTargetLockWidget->AddToViewport();
	}
}

void UEldenTargetLock::SetTargetLockWidgetPosition()
{
	if (!DrawnTargetLockWidget || !CurrentLockedActor)
	{
		CancelTargetLockAbility();
		return;
	}

	FVector2D ScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetHeroControllerFromActorInfo(),
		CurrentLockedActor->GetActorLocation(),
		ScreenPosition,
		true
	);

	if (TargetLockWidgetSize == FVector2D::ZeroVector)
	{
		DrawnTargetLockWidget->WidgetTree->ForEachWidget(
			[this](UWidget* FoundWidget)
			{
				if (USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget))
				{
					TargetLockWidgetSize.X = FoundSizeBox->GetWidthOverride();
					TargetLockWidgetSize.Y = FoundSizeBox->GetHeightOverride();
				}
			}
		);
	}

	ScreenPosition -= (TargetLockWidgetSize / 2.f);

	DrawnTargetLockWidget->SetPositionInViewport(ScreenPosition, false);
}

void UEldenTargetLock::InitTargetLockMovement()
{
	CachedDefaultMaxWalkSpeed = GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed;

	GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = TargetLockMaxWalkSpeed;
}

void UEldenTargetLock::InitTargetLockMappingContext()
{
	const ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem)

	Subsystem->AddMappingContext(TargetLockMappingContext, 3);
}

void UEldenTargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UEldenTargetLock::CleanUp()
{
	AvailableActorsToLock.Empty();

	CurrentLockedActor = nullptr;

	if (DrawnTargetLockWidget)
	{
		DrawnTargetLockWidget->RemoveFromParent();
	}

	DrawnTargetLockWidget = nullptr;

	TargetLockWidgetSize = FVector2D::ZeroVector;

	CachedDefaultMaxWalkSpeed = 0.f;
}

void UEldenTargetLock::ResetTargetLockMovement()
{
	if (CachedDefaultMaxWalkSpeed > 0.f)
	{
		GetHeroCharacterFromActorInfo()->GetCharacterMovement()->MaxWalkSpeed = CachedDefaultMaxWalkSpeed;
	}
}

void UEldenTargetLock::ResetTargetLockMappingContext()
{
	if (!GetHeroControllerFromActorInfo())
	{
		return;
	}

	const ULocalPlayer* LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem)

	Subsystem->RemoveMappingContext(TargetLockMappingContext);
}
