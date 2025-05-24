// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "Interface/HitInterface.h"
#include "Interface/PawnCombatInterface.h"
#include "BaseCharacter.generated.h"

class UEldenAbilitySystemComponent;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;
class UAttributeComponent;
class AWeapon;
enum class ECharacterClass : uint8;

UCLASS()
class ELDENLORD_API ABaseCharacter : public ACharacter, public IHitInterface, public IAbilitySystemInterface,
                                     public ICombatInterface, public IPawnCombatInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	/*  Combat Interface  */
	UAttributeSet* GetAttributeSet() const { return AttributeSet; };
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual UAnimMontage* GetThrowAwayMontage_Implementation() override;
	virtual UAnimMontage* GetStandUpMontage_Implementation() override;
	virtual UAnimMontage* GetKnockDownMontage_Implementation() override;
	virtual UAnimMontage* GetAttackMontage_Implementation() override;
	virtual UAnimMontage* GetSummonMontage_Implementation() override;
	virtual UAnimMontage* GetCarryMontage_Implementation() override;
	virtual TArray<FName> GetAttackMontageSection_Implementation() override;
	virtual void Die() override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual bool IsDead_Implementation() const override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void UpdateMinionCount_Implementation(int32 Amount) override;
	virtual int32 UpdateAttackCount_Implementation() override;
	virtual TSubclassOf<AActor> GetWeapon_Implementation() override;
	/*  End Combat Interface  */
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(BlueprintReadWrite)
	bool IsAttacking;
	
	UPROPERTY(BlueprintReadWrite)
	bool SaveAttack;
	
	UPROPERTY(BlueprintReadWrite)
	bool AttackCount;
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled, AWeapon* NewWeapon);

	UFUNCTION(BlueprintCallable)
	void SetCharacterCollisionResponse(ECollisionResponse CollisionResponse);
	
	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> SpellWeapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponMeleeTipSocketName;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponSecondMeleeTipSocketName;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AActor> EquippedWeapon;

	virtual FVector GetCombatSocketLocation_Implementation();

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	bool bDead = false;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category= "Attributs")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttribute;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category= "Attributs")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttribute;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category= "Attributs")
	TSubclassOf<UGameplayEffect> DefaultVitalAttribute;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> DefaultAttributeClass, float Level) const;

	virtual void InitializeDefaultAttribute() const;

	void AddCharacterAbilities() const;

	/* Dissolve Effect */

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	int32 MinionsCount = 0;
private:
	UPROPERTY(EditAnywhere, Category=Abilities)
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities;

	/*
	 * Montages To Play
	 */
	
	UPROPERTY(EditAnywhere, Category=Combat)
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(EditAnywhere, Category=Combat)
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditAnywhere, Category=Combat)
	TObjectPtr<UAnimMontage> SummonMontage;
	
	UPROPERTY(EditAnywhere, Category=Combat)
	TObjectPtr<UAnimMontage> ThrowAwayMontage;
	
	UPROPERTY(EditAnywhere, Category=Combat)
	TObjectPtr<UAnimMontage> StandUpMontage;
	
	UPROPERTY(EditAnywhere, Category=Combat)
	TObjectPtr<UAnimMontage> KnockDownMontage;
	
	UPROPERTY(EditAnywhere, Category=Combat)
	TObjectPtr<UAnimMontage> CarryMontage;
	
	UPROPERTY(EditAnywhere, Category=Combat)
	TArray<FName> AttackMontageSection;

public:
	FORCEINLINE UAbilitySystemComponent* GetEldenAbilitySystemComponent() const {return AbilitySystemComponent;}
};
