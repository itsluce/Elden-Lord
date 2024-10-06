// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "BaseCharacter.generated.h"

enum EDeathPose : int;
class UAttributeComponent;
class AWeapon;

UCLASS()
class ELDENLORD_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(ECollisionEnabled::Type CollisionEnabled);

protected:
	virtual void BeginPlay() override;
	void SpawnHitBlood(const FVector& ImpactPoint);
	void PlayHitSound(const FVector& ImpactPoint);
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	// virtual void GetStun_Implementation(bool bStun) override;
	virtual void Attack();
	virtual void HandleDamage(float DamageAmount);
	virtual bool CanAttack();
	virtual bool IsAlive();
	virtual void Die();
	void DisableCollision();
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	void QuitGame();

	/*
	 * Play Montage Function
	 */
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	void PlayHitReactMontage(const FName& SectionName);
	void DirectionalHitReact(const FVector& ImpactPoint);
	void StopAttackMontage();

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	bool bStunAttack = false;

	UPROPERTY(VisibleAnywhere, Category=Weapon)
	AWeapon* EquipWeapon;

	UPROPERTY(VisibleAnywhere, Category=Weapon)
	AWeapon* EquipWeapon1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAttributeComponent* Attributes;

	/*
	 * Animation montage
	 */
	UPROPERTY(EditDefaultsOnly, Category=Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathMontageSections;

	UPROPERTY(BlueprintReadOnly)
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category=Combat)
	double WarpTargetDistance = 75.f;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(EditAnywhere, Category=Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category=VFX)
	UParticleSystem* HitParticles;

public:
	FORCEINLINE EDeathPose GetDeathPose() const { return DeathPose; }
};
