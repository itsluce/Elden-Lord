// luce copyright


#include "Character/BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/EldenAbilitySystemComponent.h"
#include "AbilitySystem/EldenAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Components/EldenCombatComponent.h"
#include "EldenDbug.h"
#include "EldenLord/EldenLord.h"
#include "Item/Weapon/Weapon.h"
#include "MotionWarpingComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpellWeapon = CreateDefaultSubobject<USkeletalMeshComponent>("Spell Weapon");
	SpellWeapon->SetupAttachment(GetMesh(), FName("Hand_RSocket"));
	SpellWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	 MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

UAnimMontage* ABaseCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

UAnimMontage* ABaseCharacter::GetThrowAwayMontage_Implementation()
{
	return ThrowAwayMontage;
}

UAnimMontage* ABaseCharacter::GetStandUpMontage_Implementation()
{
	return StandUpMontage;
}

UAnimMontage* ABaseCharacter::GetKnockDownMontage_Implementation()
{
	return KnockDownMontage;
}

UAnimMontage* ABaseCharacter::GetAttackMontage_Implementation()
{
	return AttackMontage;
}

UAnimMontage* ABaseCharacter::GetSummonMontage_Implementation()
{
	return SummonMontage;
}

UAnimMontage* ABaseCharacter::GetCarryMontage_Implementation()
{
	return CarryMontage;
}

TSubclassOf<AWeapon> ABaseCharacter::SpawnedWeaponClass_Implementation()
{
	return WeaponClass;
}

TArray<FName> ABaseCharacter::GetAttackMontageSection_Implementation()
{
	return AttackMontageSection;
}

FName ABaseCharacter::GetWeaponSocket_Implementation()
{
	return WeaponSocket;
}


void ABaseCharacter::Die()
{
	// if (MeleeWeapon)
	// {
	// 	MeleeWeapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	// }
	if (SpellWeapon)
	{
		SpellWeapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	}
	MulticastHandleDeath();
}

AActor* ABaseCharacter::GetAvatar_Implementation()
{
	return this;
}

bool ABaseCharacter::IsDead_Implementation() const
{
	return bDead;
}

int32 ABaseCharacter::GetMinionCount_Implementation()
{
	return MinionsCount;
}

void ABaseCharacter::UpdateMinionCount_Implementation(int32 Amount)
{
	MinionsCount += Amount;
}

UPawnCombatComponent* ABaseCharacter::GetPawnCombatComponent() const
{
	return nullptr;
}


void ABaseCharacter::MulticastHandleDeath_Implementation()
{
	// if (MeleeWeapon)
	// {
	// 	if (AActor* ChildActor = MeleeWeapon->GetChildActor())
	// 	{
	// 		AWeapon* Weapon = Cast<AWeapon>(ChildActor);
	// 		Weapon->GetWeaponMesh()->SetSimulatePhysics(true);
	// 		Weapon->GetWeaponMesh()->SetEnableGravity(true);
	// 		Weapon->GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	// 	}
	// }
	if (SpellWeapon)
	{
		SpellWeapon->SetSimulatePhysics(true);
		SpellWeapon->SetEnableGravity(true);
		SpellWeapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Dissolve();
	FGameplayEventData Data;
	Data.Instigator = GetOwner();
	
	bDead = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

FVector ABaseCharacter::GetCombatSocketLocation_Implementation()
{
	if (SpellWeapon && CharacterClass != ECharacterClass::Warrior)
	{
		return SpellWeapon->GetSocketLocation(WeaponTipSocketName);
	}
	// if (MeleeWeapon && CharacterClass == ECharacterClass::Warrior)
	// {
	// 	return MeleeWeapon->GetSocketLocation(WeaponMeleeTipSocketName);
	// }
	return FVector();
}

void ABaseCharacter::InitAbilityActorInfo()
{
}

void ABaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> DefaultAttributeClass, float Level) const
{
	checkf(IsValid(GetAbilitySystemComponent()), TEXT("You must initialize a valid GetAbilitySystemComponent"));
	checkf(DefaultAttributeClass, TEXT("You must initialize a valid DefaultAttribute"));
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
		DefaultAttributeClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ABaseCharacter::InitializeDefaultAttribute() const
{
	ApplyEffectToSelf(DefaultPrimaryAttribute, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttribute, 1.f);
	ApplyEffectToSelf(DefaultVitalAttribute, 1.f);
}

void ABaseCharacter::AddCharacterAbilities() const
{
	if (!HasAuthority())return;
	UEldenAbilitySystemComponent* ASC = Cast<UEldenAbilitySystemComponent>(AbilitySystemComponent);
	ASC->AddCharacterAttributes(StartUpAbilities);
}

void ABaseCharacter::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);

		StartDissolveTimeline(DynamicMatInst);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(
			WeaponDissolveMaterialInstance, this);
		SpellWeapon->SetMaterial(0, DynamicMatInst);

		// if (AActor* ChildActor = MeleeWeapon->GetChildActor())
		// {
		// 	AWeapon* Weapon = Cast<AWeapon>(ChildActor);
		// 	Weapon->GetWeaponMesh()->SetMaterial(0, DynamicMatInst);
		// }


		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::SetCharacterCollisionResponse(ECollisionResponse CollisionResponse)
{
	if (GetMesh())
	{
		GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, CollisionResponse);
	}
}

void ABaseCharacter::StartStaminaRegenTimer()
{
	// Clear any existing timers
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenDelayTimer);
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimer);
	
	//Debug::Print(FString::Printf(TEXT("Starting stamina regeneration timer (%.1f sec delay)"), StaminaRegenDelay), FColor::Cyan);
	
	// Start the delay timer (2 seconds before regen begins)
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenDelayTimer, this, &ABaseCharacter::BeginStaminaRegeneration, StaminaRegenDelay, false);
}

void ABaseCharacter::BeginStaminaRegeneration()
{
	//Debug::Print(TEXT("Stamina regeneration starting!"), FColor::Green);
	
	// Start the regeneration timer
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimer, this, &ABaseCharacter::StaminaRegenTick, StaminaRegenTickRate, true);
}

void ABaseCharacter::StaminaRegenTick()
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	
	const float CurrentStamina = AbilitySystemComponent->GetNumericAttribute(UEldenAttributeSet::GetStaminaAttribute());
	const float MaxStamina = AbilitySystemComponent->GetNumericAttribute(UEldenAttributeSet::GetMaxStaminaAttribute());
	
	// Stop regeneration if stamina is already at max
	if (CurrentStamina >= MaxStamina)
	{
		//Debug::Print(FString::Printf(TEXT("Stamina regeneration complete! Stamina: %.1f/%.1f"), CurrentStamina, MaxStamina), FColor::Green);
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimer);
		return;
	}
	
	// Calculate regeneration amount per tick
	const float RegenAmount = StaminaRegenRate * StaminaRegenTickRate;
	const float NewStamina = FMath::Min(MaxStamina, CurrentStamina + RegenAmount);
	
	// Apply the regeneration
	AbilitySystemComponent->SetNumericAttributeBase(UEldenAttributeSet::GetStaminaAttribute(), NewStamina);
	
	// Debug print regeneration tick
	//Debug::Print(FString::Printf(TEXT("Stamina regen: %.1f -> %.1f (%.1f/%.1f)"), CurrentStamina, NewStamina, NewStamina, MaxStamina), FColor::Blue);
}