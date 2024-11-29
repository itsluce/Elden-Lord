// luce copyright


#include "Character/BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "Character/CharacterTypes.h"
#include "Components/AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Item/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// PrimaryActorTick.bStartWithTickEnabled = true;
	// GetMesh()->bReceivesDecals = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::InitAbilityActorInfo()
{
}

void ABaseCharacter::SpawnHitBlood(const FVector& ImpactPoint)
{
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
	}
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
}

void ABaseCharacter::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	PlayHitSound(ImpactPoint);
	SpawnHitBlood(ImpactPoint);
	if (Attributes && Attributes->IsAlive())
	{
		if (Hitter)
			DirectionalHitReact(Hitter->GetActorLocation());
	}
	else
	{
		DirectionalHitReact(Hitter->GetActorLocation());
		Die();
		// QuitGame();
	}
}

void ABaseCharacter::Attack()
{
	if (CombatTarget && CombatTarget->ActorHasTag("Dead"))
	{
		CombatTarget = nullptr;
	}
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes) Attributes->ReceiveDamage(DamageAmount);
}

int32 ABaseCharacter::PlayAttackMontage()
{
	const int32 Selection = PlayRandomMontageSection(AttackMontage, AttackMontageSections);
	return Selection;
}

int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDP_MAX)
	{
		DeathPose = Pose;
	}
	return Selection;
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::Die()
{
	Tags.Add(TEXT("Dead"));
	PlayDeathMontage();
}

void ABaseCharacter::DisableCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	if (Selection == 2)
	{
		// if ()
		// {
		// 	IHitInterface* StunInterface = Cast<IHitInterface>();
		// 	if (StunInterface)
		// 	{
		// 		StunInterface->Execute_GetStun(, true);
		// 	}
		// }
	}
	else
	{
		bStunAttack = false;
	}
	return Selection;
}

// void ABaseCharacter::GetStun_Implementation(bool bStun)
// {
// 		if (GEngine)
// 		{
// 			GEngine->AddOnScreenDebugMessage(18, 5.f, FColor::Blue,
// 											 FString::Printf(TEXT("bStun: %d"), bStun));
// 		}
// 		if (bStun)
// 		{
// 			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
// 			if (AnimInstance && AttackMontage)
// 			{
// 				AnimInstance->Montage_Play(AttackMontage);
// 				AnimInstance->Montage_JumpToSection(FName("StunHitReact"), AttackMontage);
// 			}
// 		}
// }
void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	// Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();
	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);
	// if CrossProduct points down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	// if (GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(13, 5.f, FColor::Red, FString::Printf(TEXT("bStunAttack: %d"), bStunAttack));
	// }
	// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f,
	//                                      FColor::Blue, 5.f);
	//
	// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f,
	//                                      FColor::Red, 5.f);
	// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f,
	//                                      FColor::Green, 5.f);
	PlayHitReactMontage(Section);
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
	}
}

FVector ABaseCharacter::GetTranslationWarpTarget()
{
	if (CombatTarget == nullptr)return FVector();
	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();
	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;
	return CombatTargetLocation + TargetToMe;
}

FVector ABaseCharacter::GetRotationWarpTarget()
{
	if (CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

void ABaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> DefaultAttributeClass, float Level) const
{
	checkf(IsValid(GetAbilitySystemComponent()), TEXT("You must initialize a valid GetAbilitySystemComponent"));
	checkf(DefaultAttributeClass, TEXT("You must initialize a valid DefaultAttribute"));
	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
		DefaultAttributeClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ABaseCharacter::InitializeDefaultAttribute() const
{
	ApplyEffectToSelf(DefaultPrimaryAttribute, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttribute, 1.f);
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetWeaponCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquipWeapon && EquipWeapon->GetWeaponBox())
	{
		EquipWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquipWeapon->IgnoreActors.Empty();
	}
	if (EquipWeapon1 && EquipWeapon1->GetWeaponBox())
	{
		EquipWeapon1->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquipWeapon1->IgnoreActors.Empty();
	}
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
