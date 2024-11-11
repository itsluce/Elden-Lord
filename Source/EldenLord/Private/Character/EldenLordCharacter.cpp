// luce copyright


#include "Character/EldenLordCharacter.h"

#include "AbilitySystemComponent.h"
#include "EldenDbug.h"
#include "AbilitySystem/EldenAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AttributeComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HUD/EldenOverlay.h"
#include "Item/Weapon/Weapon.h"
#include "HUD/EldenHUD.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/EldenController.h"
#include "Player/EldenPlayerState.h"

AEldenLordCharacter::AEldenLordCharacter()
{
	PrimaryActorTick.bCanEverTick = true;


	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	SpringArm1 = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm1->SetupAttachment(GetRootComponent());
	Camera1 = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera1->SetupAttachment(SpringArm1);

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	// GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	// GetCharacterMovement()->bConstrainToPlane = true;
	// GetCharacterMovement()->bSnapToPlaneAtStart = true;

	MainWeapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	MainWeapon->SetupAttachment(GetMesh(), FName("Hand_RSocket"));
	MainWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// bIsWaitingForSecondPress = false;
	// FirstPressTime = 0.0f;
	// DoublePressThreshold = 0.3f;
	CurrentComboIndex = 0;
	AttackSectionNames[0] = "Attack1";
	AttackSectionNames[1] = "Attack2";

	AttackSectionNames[2] = "Attack3";
}

void AEldenLordCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("EldenLord"));
	InitializeOverlay();
	// Debug::Print(TEXT("hello"));
}

void AEldenLordCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// if (GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("State: %d"), CurrentComboIndex));
	// 	GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Green, FString::Printf(TEXT("State: %d"), TriggerNextAttack));
	// }
	if (CurrentComboIndex > 2)
	{
		CurrentComboIndex = 0;
	}
	if (Attributes)
	{
		Attributes->RegenStamina(DeltaTime);
		EldenOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

void AEldenLordCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	SetWeaponCollision(ECollisionEnabled::NoCollision);
	CurrentComboIndex = 0;
	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

void AEldenLordCharacter::InitAbilityActorInfo()
{
	AEldenPlayerState* EldenPlayerState = GetPlayerState<AEldenPlayerState>();
	check(EldenPlayerState);
	EldenPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(EldenPlayerState, this);
	Cast<UEldenAbilitySystemComponent>(EldenPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = EldenPlayerState->GetAbilitySystemComponent();
	AttributeSet = EldenPlayerState->GetAttributeSet();

	if (AEldenController* EldenController = Cast<AEldenController>(GetController()))
	{
		if (AEldenHUD* EldenHUD = Cast<AEldenHUD>(EldenController->GetHUD()))
		{
			EldenHUD->InitOverlay(EldenController,EldenPlayerState,AbilitySystemComponent,AttributeSet);
		}
		
	}
}

void AEldenLordCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the server
	InitAbilityActorInfo();
}

void AEldenLordCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the server
	InitAbilityActorInfo();
}


void AEldenLordCharacter::InitializeOverlay()
{
	// APlayerController* PlayerController = Cast<APlayerController>(GetController());
	//
	// if (PlayerController)
	// {
	// 	AEldenHUD* EldenHUD = Cast<AEldenHUD>(PlayerController->GetHUD());
	// 	if (EldenHUD)
	// 	{
	// 		EldenOverlay = EldenHUD->GetEldenOverlay();
	// 		if (EldenOverlay && Attributes)
	// 		{
	// 			EldenOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	// 			EldenOverlay->SetStaminaBarPercent(1.f);
	// 			EldenOverlay->SetGolds(0);
	// 			EldenOverlay->SetSouls(0);
	// 		}
	// 	}
	// }
}

void AEldenLordCharacter::MoveForward(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if (Controller && Value != 0.f)
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AEldenLordCharacter::TurnRight(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if (Controller && Value != 0)
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AEldenLordCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AEldenLordCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}


void AEldenLordCharacter::EKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (!OverlappingWeapon)
	{
		return;
	}
	OverlappingWeapon->SetInstigator(this);
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("Hand_RSocket"), this, this);
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr;
		EquipWeapon = OverlappingWeapon;
	}
	// if (OverlappingHorses)
	// {
	// 	OverlappingHorses->Equip(GetMesh(), FName("HorseSocket"));
	// 	CharacterState = ECharacterState::ECS_EquippedHorse;
	// 	// OverlappingItem = nullptr;
	// 	// EquipWeapon = OverlappingWeapon;
	// }
}


void AEldenLordCharacter::RKeyPressed()
{
	if (Controller && Attributes && Attributes->GetHealthPotion() > 0 && EldenOverlay &&
		ActionState == EActionState::EAS_Unoccupied)
	{
		ActionState = EActionState::EAS_Healing;

		Attributes->UseHeal(Attributes->GetHealRegen());
		EldenOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && DrinkMontage)
		{
			AnimInstance->Montage_Play(DrinkMontage);
		}
		const int32 HealthPotionNumber = Attributes->SetHealthPotion(Attributes->GetHealthPotion());
		EldenOverlay->SetHealthPotionNumber(HealthPotionNumber);
	}
}

void AEldenLordCharacter::ResetDoublePress()
{
	bIsWaitingForSecondPress = false;
}

void AEldenLordCharacter::Jump()
{
	// if (!bIsWaitingForSecondPress)
	// {
	// 	const float CurrentTime = GetWorld()->GetTimeSeconds();
	// 	FirstPressTime = GetWorld()->GetTimeSeconds();
	// 	if (CurrentTime - FirstPressTime > DoublePressThreshold)
	// 	{
	// 		if (ActionState != EActionState::EAS_Dead && ActionState != EActionState::EAS_HitReaction)
	// 		{
	// 			Super::Jump();
	// 		}
	// 		GetWorld()->GetTimerManager().SetTimer(DoublePressTimerHandle, this, &AEldenLordCharacter::ResetDoublePress,
	// 		                                       DoublePressThreshold, false);
	// 	}
	// }
	// else
	// {
	// 	ActionState = EActionState::EAS_Dodge;
	// 	const float CurrentTime = GetWorld()->GetTimeSeconds();
	// 	if (CurrentTime - FirstPressTime <= DoublePressThreshold)
	// 	{
	// 		Dodge();
	// 		bIsWaitingForSecondPress = false;
	// 		GetWorld()->GetTimerManager().ClearTimer(DoublePressTimerHandle);
	// 	}
	// 	else
	// 	{
	// 		bIsWaitingForSecondPress = false;
	// 	}
	// }
}

bool AEldenLordCharacter::HasDodgeStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}

bool AEldenLordCharacter::HasAttackStamina()
{
	return Attributes && Attributes->GetStamina() > 0;
}

void AEldenLordCharacter::Dodge()
{
	if (ActionState == EActionState::EAS_Dodge || ActionState == EActionState::EAS_Dead || !HasDodgeStamina())return;
	ActionState = EActionState::EAS_Dodge;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DodgeMontage)
	{
		AnimInstance->Montage_Play(DodgeMontage);
	}
	if (Attributes && EldenOverlay)
	{
		Attributes->UseStamina(Attributes->GetDodgeCost());
		EldenOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}


void AEldenLordCharacter::DodgeDistance()
{
	FVector ForwardDirection = GetActorForwardVector();
	LaunchCharacter(ForwardDirection * DodgeStrength, true, true);
}

void AEldenLordCharacter::DodgeEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AEldenLordCharacter::DrinkPotionEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}


bool AEldenLordCharacter::CanDisArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}


bool AEldenLordCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquipWeapon;
}


void AEldenLordCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AEldenLordCharacter::DisArmEnd()
{
	if (EquipWeapon)
	{
		EquipWeapon->AttachMeshToSocket(GetMesh(), FName("AttachWeaponSocket"));
	}
}

void AEldenLordCharacter::ArmEnd()
{
	if (EquipWeapon)
	{
		EquipWeapon->AttachMeshToSocket(GetMesh(), FName("Hand_RSocket"));
	}
}

void AEldenLordCharacter::ArmFinish()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AEldenLordCharacter::Armed()
{
	if (CanDisArm())
	{
		PlayArmedMontage(FName("UnEquip"));
		CharacterState = ECharacterState::ECS_Unequipped;
		ActionState = EActionState::EAS_EquippingWeapon;
	}
	else if (CanArm())
	{
		PlayArmedMontage(FName("Equip"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		ActionState = EActionState::EAS_EquippingWeapon;
	}
}

void AEldenLordCharacter::PlayArmedMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ArmedMontage)
	{
		AnimInstance->Montage_Play(ArmedMontage);
		AnimInstance->Montage_JumpToSection(SectionName, ArmedMontage);
	}
}

void AEldenLordCharacter::NextAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(AttackSectionNames[CurrentComboIndex], AttackMontage);
	}
	if (Attributes)
	{
		Attributes->UseStamina(Attributes->GetAttackCost());
		EldenOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

void AEldenLordCharacter::Attack()
{
	Super::Attack();
	if (CanAttack())
	{
		ActionState = EActionState::EAS_Attacking;
		NextAttackMontage();
	}
	else if (ActionState == EActionState::EAS_Attacking)
	{
		if (CurrentComboIndex <= 2)
		{
			CurrentComboIndex++;
			TriggerNextAttack = true;
		}
		else
		{
			CurrentComboIndex = 0;
		}
	}
}

void AEldenLordCharacter::AttackEnd()
{
	if (!TriggerNextAttack)
	{
		ActionState = EActionState::EAS_Unoccupied;
		CurrentComboIndex = 0;
	}
}

void AEldenLordCharacter::NextAttackEnd()
{
	if (TriggerNextAttack && CurrentComboIndex <= 2)
	{
		TriggerNextAttack = false;
		NextAttackMontage();
	}
	else
	{
		ActionState = EActionState::EAS_Unoccupied;
		CurrentComboIndex = 0;
	}
}

bool AEldenLordCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped &&
		ActionState != EActionState::EAS_HitReaction &&
		HasAttackStamina();
}

void AEldenLordCharacter::Die()
{
	Super::Die();
	ActionState = EActionState::EAS_Dead;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEldenLordCharacter::SetHUDHealth()
{
	if (EldenOverlay && Attributes)
	{
		EldenOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

float AEldenLordCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                      AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void AEldenLordCharacter::SetCharacterCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (GetMesh())
	{
		GetMesh()->SetCollisionEnabled(CollisionEnabled);
	}
}

void AEldenLordCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AEldenLordCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AEldenLordCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &AEldenLordCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("TurnRight"), this, &AEldenLordCharacter::TurnRight);

	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &AEldenLordCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &AEldenLordCharacter::Attack);
	PlayerInputComponent->BindAction(FName("Armed"), IE_Pressed, this, &AEldenLordCharacter::Armed);
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &AEldenLordCharacter::Dodge);
	PlayerInputComponent->BindAction(FName("HealthRegen"), IE_Pressed, this, &AEldenLordCharacter::RKeyPressed);
}
