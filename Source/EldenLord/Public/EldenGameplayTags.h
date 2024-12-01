// luce copyright

#pragma once

#include "CoreMinimal.h"
#include  "GameplayTagContainer.h"
/**
 * 
 */

struct FEldenGameplayTags
{
public:
	static const FEldenGameplayTags& Get() { return GameplayTags; }
	static void InitializedNativeGameplayTags();
	
	FGameplayTag Attributes_Primary_Vigor;
	FGameplayTag Attributes_Primary_Mind;
	FGameplayTag Attributes_Primary_Endurance;
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;

	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	FGameplayTag Attributes_Secondary_MaxStamina;
protected:

private:
	static FEldenGameplayTags GameplayTags;
};
