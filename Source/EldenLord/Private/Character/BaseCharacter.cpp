// luce copyright


#include "Character/BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/EldenAbilitySystemComponent.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Components/CapsuleComponent.h"
#include "EldenLord/EldenLord.h"
#include "Components/BoxComponent.h"
#include "Item/Weapon/Weapon.h"
#include "Item/Item.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// PrimaryActorTick.bStartWithTickEnabled = true;
	// GetMesh()->bReceivesDecals = false;

	SpellWeapon = CreateDefaultSubobject<USkeletalMeshComponent>("Spell Weapon");
	SpellWeapon->SetupAttachment(GetMesh(), FName("Hand_RSocket"));
	SpellWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MeleeWeapon = CreateDefaultSubobject<UChildActorComponent>("Melee Weapon");
	MeleeWeapon->SetupAttachment(GetMesh(), FName("Hand_LSocket"));
	
	// MainWeapon = CreateDefaultSubobject<USkeletalMeshComponent>("Main Weapon");
	// MainWeapon->SetupAttachment(GetMesh(), FName("Hand_LSocket"));
	// MainWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
}

UAnimMontage* ABaseCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ABaseCharacter::Die()
{
	if (MeleeWeapon)
	{
		MeleeWeapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	}
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


void ABaseCharacter::MulticastHandleDeath_Implementation()
{
	if (MeleeWeapon)
	{
		if (AActor* ChildActor = MeleeWeapon->GetChildActor())
		{
			AWeapon* Weapon = Cast<AWeapon>(ChildActor);
			Weapon->GetWeaponMesh()->SetSimulatePhysics(true);
			Weapon->GetWeaponMesh()->SetEnableGravity(true);
			Weapon->GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		}
	}
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
	
	bDead = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

FVector ABaseCharacter::GetCombatSocketLocation_Implementation()
{
	if (SpellWeapon && CharacterClass == ECharacterClass::Ranger)
	{
		return SpellWeapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (MeleeWeapon && CharacterClass == ECharacterClass::Warrior)
	{
		return MeleeWeapon->GetSocketLocation(WeaponMeleeTipSocketName);
	}
	return FVector();
}

void ABaseCharacter::InitAbilityActorInfo()
{
}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (AActor* ChildActor = MeleeWeapon->GetChildActor())
	{
		AWeapon* Weapon = Cast<AWeapon>(ChildActor);
		if (Weapon && Weapon->GetWeaponBox())
		{
			Weapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
			Weapon->IgnoreActors.Empty();
		}
	}
	
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
		
		if (AActor* ChildActor = MeleeWeapon->GetChildActor())
		{
			AWeapon* Weapon = Cast<AWeapon>(ChildActor);
			Weapon->GetWeaponMesh()->SetMaterial(0, DynamicMatInst);
		}
		

		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
