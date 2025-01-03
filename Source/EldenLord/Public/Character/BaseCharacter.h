// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "Interface/EnemyInterface.h"
#include "Interface/HitInterface.h"
#include "BaseCharacter.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;
class UAttributeComponent;
class AWeapon;
enum class ECharacterClass : uint8;

UCLASS()
class ELDENLORD_API ABaseCharacter : public ACharacter, public IHitInterface, public IAbilitySystemInterface,
                                     public ICombatInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	/*  Combat Interface  */
	UAttributeSet* GetAttributeSet() const { return AttributeSet; };
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual UAnimMontage* GetAttackMontage_Implementation() override;
	virtual TArray<FName> GetAttackMontageSection_Implementation() override;
	virtual void Die() override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual bool IsDead_Implementation() const override;
	/*  End Combat Interface  */
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	UFUNCTION(BlueprintCallable)
	void SetCharacterCollisionResponse(ECollisionResponse CollisionResponse);
	/*
	 * Play Montage Function
	 */
	
	UPROPERTY(EditAnywhere, Category="Combat")
	UChildActorComponent* MeleeWeapon;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> SpellWeapon;

	// UPROPERTY(EditAnywhere, Category="Combat")
	// TObjectPtr<USkeletalMeshComponent> MainWeapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponMeleeTipSocketName;

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

private:
	UPROPERTY(EditAnywhere, Category=Abilities)
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities;

	UPROPERTY(EditAnywhere, Category=Combat)
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(EditAnywhere, Category=Combat)
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditAnywhere, Category=Combat)
	TArray<FName> AttackMontageSection;
};
