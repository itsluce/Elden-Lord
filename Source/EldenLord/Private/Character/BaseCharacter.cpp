// luce copyright


#include "Character/BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/EldenAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "EldenLord/EldenLord.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// PrimaryActorTick.bStartWithTickEnabled = true;
	// GetMesh()->bReceivesDecals = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile,ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
}

UAnimMontage* ABaseCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

FVector ABaseCharacter::GetCombatSocketLocation()
{
	check(SpellWeapon);
	return SpellWeapon->GetSocketLocation(WeaponTipSocketName);
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

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
