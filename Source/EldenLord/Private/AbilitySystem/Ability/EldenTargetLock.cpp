// luce copyright


#include "AbilitySystem/Ability/EldenTargetLock.h"
#include "EldenDbug.h"
#include "EldenGameplayTags.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "UI/Widget/EldenUserWidget.h"
#include "UI/Controller/EldenWidgetController.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"
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
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UEldenTargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                  bool bWasCancelled)
{
	CleanUp();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEldenTargetLock::TryLockOnTarget()
{
	GetAvailableTargetToLock();
	if (AvailableTargetActorsToLock.IsEmpty())
	{
		CancelTargetToLockAbility();
		return;
	}
	CurrentActorToLock = GetNearestAvailableTargetToLock(AvailableTargetActorsToLock);
	if (CurrentActorToLock)
	{
		Debug::Print(CurrentActorToLock->GetActorNameOrLabel());
		DrawTargetToLockWidget();
		SetTargetToLockWidgetPosition();
	}
	else
	{
		CancelTargetToLockAbility();
	}
}

void UEldenTargetLock::GetAvailableTargetToLock()
{
	TArray<FHitResult> BoxTraceHitResults;
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetAvatarActorFromActorInfo(),
		GetAvatarActorFromActorInfo()->GetActorLocation(),
		GetAvatarActorFromActorInfo()->GetActorLocation() +
		GetAvatarActorFromActorInfo()->GetActorForwardVector() *
		BoxTraceDistance,
		TraceBoxSize / 2.f,
		GetAvatarActorFromActorInfo()->GetActorForwardVector().
		                               ToOrientationRotator(),
		BoxTraceChannel,
		false,
		TArray<AActor*>(),
		bShowDebug ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		BoxTraceHitResults,
		true
	);
	for (const FHitResult& HitResult : BoxTraceHitResults)
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (HitActor != GetAvatarActorFromActorInfo())
			{
				AvailableTargetActorsToLock.AddUnique(HitActor);
				// Debug::Print(HitActor->GetActorNameOrLabel());
			}
		}
	}
}

AActor* UEldenTargetLock::GetNearestAvailableTargetToLock(const TArray<AActor*>& FoundTargets)
{
	float ClosestDistance = 0.f;
	return UGameplayStatics::FindNearestActor(GetAvatarActorFromActorInfo()->GetActorLocation(), FoundTargets,
	                                          ClosestDistance);
}

void UEldenTargetLock::DrawTargetToLockWidget()
{
	if (!WidgetToDraw)
	{
		checkf(TargetLockWidgetClass, TEXT("Target Lock Widget Class"));

		WidgetToDraw = CreateWidget<UEldenUserWidget>(GetWorld(), TargetLockWidgetClass);

		WidgetToDraw->AddToViewport();
	}
}

void UEldenTargetLock::SetTargetToLockWidgetPosition()
{
	if (!WidgetToDraw || !CurrentActorToLock)
	{
		CancelTargetToLockAbility();
		return;
	}
	FVector2d ScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetCurrentActorInfo()->PlayerController.Get(),
		CurrentActorToLock->GetActorLocation(),
		ScreenPosition,
		true
	);
	if (TargetLockWidgetSize == FVector2D::ZeroVector)
	{
		WidgetToDraw->WidgetTree->ForEachWidget(
			[this](UWidget* Widget)
			{
				if (USizeBox* SizeBox = Cast<USizeBox>(Widget))
				{
					TargetLockWidgetSize.X = SizeBox->GetWidthOverride();
					TargetLockWidgetSize.Y = SizeBox->GetHeightOverride();
				}
			}
		);
	}
	ScreenPosition -= (TargetLockWidgetSize / 2.f);
	WidgetToDraw->SetPositionInViewport(ScreenPosition, false);
}

void UEldenTargetLock::OnLockTargetTick(float DeltaTime)
{
	ICombatInterface* EldenInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	ICombatInterface* CurrentTargetInterface = Cast<ICombatInterface>(CurrentActorToLock);

	if (!CurrentActorToLock || EldenInterface->Execute_IsDead(GetAvatarActorFromActorInfo()) || CurrentTargetInterface->
		Execute_IsDead(CurrentActorToLock))
	{
		CancelTargetToLockAbility();
		return;
	}
	SetTargetToLockWidgetPosition();

	//  TODO const bool bShouldOverrideRotation =
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
		GetAvatarActorFromActorInfo()->GetActorLocation(), CurrentActorToLock->GetActorLocation());
	AEldenController* EldenController = Cast<AEldenController>(CurrentActorInfo->PlayerController);
	const FRotator CurrentControlRotation = EldenController->GetControlRotation();
	const FRotator TargetRotation = FMath::RInterpTo(CurrentControlRotation, LookAtRotation, DeltaTime,TargetInterpSpeed);
	EldenController->SetControlRotation(FRotator(TargetRotation.Pitch, TargetRotation.Yaw,0.f));
	// EldenController->SetActorRotation(FRotator(0.f, TargetRotation.Yaw,0.f),ETeleportType::None);
}

void UEldenTargetLock::CancelTargetToLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UEldenTargetLock::CleanUp()
{
	AvailableTargetActorsToLock.Empty();
	CurrentActorToLock = nullptr;

	if (WidgetToDraw)
	{
		WidgetToDraw->RemoveFromParent();
	}
	WidgetToDraw = nullptr;
	TargetLockWidgetSize = FVector2D::ZeroVector;
}
