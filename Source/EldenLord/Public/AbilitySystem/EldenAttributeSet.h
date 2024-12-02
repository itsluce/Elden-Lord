// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EldenAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties()
	{
	}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

template <class T>
using TStaticFuncPointer = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class ELDENLORD_API UEldenAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UEldenAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	TMap<FGameplayTag, TStaticFuncPointer<FGameplayAttribute()>> TagToAttribute;

	/*
	 * Primary Attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Strength, Category= "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UEldenAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Intelligence, Category= "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UEldenAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Resilience, Category= "Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UEldenAttributeSet, Resilience);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Vigor, Category= "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UEldenAttributeSet, Vigor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Mind, Category= "Primary Attributes")
	FGameplayAttributeData Mind;
	ATTRIBUTE_ACCESSORS(UEldenAttributeSet, Mind);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Endurance, Category= "Primary Attributes")
	FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(UEldenAttributeSet, Endurance);

	/*
	 * Secondary Attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Armor, Category= "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UEldenAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_ArmorPenetration, Category= "Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UEldenAttributeSet, ArmorPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_BlockChance, Category= "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UEldenAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_CriticalHitChance, Category= "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UEldenAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_CriticalHitDamage, Category= "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UEldenAttributeSet, CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_CriticalHitResistance, Category= "Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UEldenAttributeSet, CriticalHitResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_HealthRegeneration, Category= "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UEldenAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_MaxHealth, Category= "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UEldenAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_MaxMana, Category= "Secondary Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UEldenAttributeSet, MaxMana)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_MaxStamina, Category= "Secondary Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UEldenAttributeSet, MaxStamina)

	/*
	 * vital Attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Health, Category= "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UEldenAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Mana, Category= "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UEldenAttributeSet, Mana)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Stamina, Category= "Vital Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UEldenAttributeSet, Stamina)

	/*
	 * End Attributes
	 */


	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData OldMaxHealth) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData OldMaxMana) const;
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData OldStamina) const;

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData OldMaxStamina) const;

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData OldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData OldVigor) const;

	UFUNCTION()
	void OnRep_Mind(const FGameplayAttributeData OldMind) const;

	UFUNCTION()
	void OnRep_Endurance(const FGameplayAttributeData OldEndurance) const;

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData OldArmor) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData OldArmorPenetration) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData OldBlockChance) const;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData OldCriticalHitChance) const;

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData OldCriticalHitDamage) const;

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData OldCriticalHitResistance) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData OldHealthRegeneration) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
};
