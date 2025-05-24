// luce copyright


#include "Player/EldenController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EldenDbug.h"
#include "EldenGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystem/EldenAbilitySystemComponent.h"
#include "AbilitySystem/EldenAbilitySystemLibrary.h"
#include "Input/EldenInputComponent.h"
#include "Interface/EnemyInterface.h"

AEldenController::AEldenController()
{
	bReplicates = true;
	HeroTeamID = FGenericTeamId(0);
}

void AEldenController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CursorTrace();
}

FGenericTeamId AEldenController::GetGenericTeamId() const
{
	return HeroTeamID;
}

void AEldenController::BeginPlay()

{
	Super::BeginPlay();

	checkf(EldenContext, TEXT("Elden Context"))

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(EldenContext, 0);
	}

	// bShowMouseCursor = true;
	// FInputModeGameOnly InputModeData;
	// SetInputMode(InputModeData);
	// DefaultMouseCursor = EMouseCursor::Default;

	// bShowMouseCursor = true;
	// FInputModeGameAndUI InputModeData;
	// InputModeData.SetHideCursorDuringCapture(false);
	// SetInputMode(InputModeData);
}

void AEldenController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	if (APawn* ControlPawn = GetPawn<APawn>())
	{
		ControlPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AEldenController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	
	if (LookAxisVector.X != 0.f)
	{
		AddYawInput(LookAxisVector.X);
	}

	if (LookAxisVector.Y != 0.f)
	{
		AddPitchInput(LookAxisVector.Y);
	}
}

void AEldenController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEldenInputComponent* EldenInputComponent = Cast<UEldenInputComponent>(InputComponent);

	EldenInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEldenController::Move);
	EldenInputComponent->BindAction(LookAxis, ETriggerEvent::Triggered, this, &AEldenController::Look);

	EldenInputComponent->BindNativeInputAction(InputConfig, FEldenGameplayTags::Get().InputTag_PickUp,
	                                           ETriggerEvent::Completed, this,
	                                           &AEldenController::Input_PickUpStonesAbility);
	EldenInputComponent->BindNativeInputAction(InputConfig, FEldenGameplayTags::Get().InputTag_SwitchTarget,
	                                           ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchTargetTriggered);
	EldenInputComponent->BindNativeInputAction(InputConfig, FEldenGameplayTags::Get().InputTag_SwitchTarget,
	                                           ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTargetComplete);

	EldenInputComponent->BindAbilityInputAction(InputConfig,this,&ThisClass::AbilityInputTagPressed,&ThisClass::AbilityInputTagReleased);
}

void AEldenController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/**
	 * Line trace from cursor. There are several scenarios:
	 *  A. LastActor is null && ThisActor is null
	 *		- Do nothing
	 *	B. LastActor is null && ThisActor is valid
	 *		- Highlight ThisActor
	 *	C. LastActor is valid && ThisActor is null
	 *		- UnHighlight LastActor
	 *	D. Both actors are valid, but LastActor != ThisActor
	 *		- UnHighlight LastActor, and Highlight ThisActor
	 *	E. Both actors are valid, and are the same actor
	 *		- Do nothing
	 */

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// Case B
			ThisActor->HighlightActor();
		}
		else
		{
			// Case A - both are null, do nothing
		}
	}
	else // LastActor is valid
	{
		if (ThisActor == nullptr)
		{
			// Case C
			LastActor->UnHighlightActor();
		}
		else // both actors are valid
		{
			if (LastActor != ThisActor)
			{
				// Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				// Case E - do nothing
			}
		}
	}
}

void AEldenController::Input_PickUpStonesAbility(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;

	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			ControlledPawn,
			FEldenGameplayTags::Get().Event_ConsumeStones,
			Data
		);
	}
}

void AEldenController::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
	SwitchDirection = InputActionValue.Get<FVector2D>();
}

void AEldenController::Input_SwitchTargetComplete(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			ControlledPawn,
			SwitchDirection.X > 0.f
				? FEldenGameplayTags::Get().Event_SwitchTarget_Right
				: FEldenGameplayTags::Get().Event_SwitchTarget_Left,
			Data
		);
	}
}

void AEldenController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() == nullptr)return;
	GetASC()->AbilityInputTagPreesed(InputTag);
}

void AEldenController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr)return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void AEldenController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() == nullptr)return;
	GetASC()->AbilityInputTagHeld(InputTag);
}

UEldenAbilitySystemComponent* AEldenController::GetASC()
{
	if (EldenAbilitySystemComponent == nullptr)
	{
		EldenAbilitySystemComponent = Cast<UEldenAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return EldenAbilitySystemComponent;
}
