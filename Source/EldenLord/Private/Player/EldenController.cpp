// luce copyright


#include "Player/EldenController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystem/EldenAbilitySystemComponent.h"
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
	const float Value = InputActionValue.Get<float>();

	// if (GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(8, 5.f, FColor::Purple, FString::Printf(TEXT("InputActionValue: %f"), Value));
	// }

	AddPitchInput(Value);
}

void AEldenController::Turn(const FInputActionValue& InputActionValue)
{
	const float Value = InputActionValue.Get<float>();
	AddYawInput(Value);
}

void AEldenController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEldenInputComponent* EldenInputComponent = Cast<UEldenInputComponent>(InputComponent);

	EldenInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEldenController::Move);
	EldenInputComponent->BindAction(LookAxis, ETriggerEvent::Triggered, this, &AEldenController::Look);
	EldenInputComponent->BindAction(TurnAxis, ETriggerEvent::Triggered, this, &AEldenController::Turn);
	EldenInputComponent->BindAbilitiesAction(InputConfig, this, &ThisClass::AbilityInputTagPressed,
	                                         &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
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

void AEldenController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	
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
