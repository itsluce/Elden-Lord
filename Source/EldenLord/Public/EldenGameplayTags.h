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
	static const FEldenGameplayTags& Get() { return GameplayTags;}
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Primary_Vigor;
	FGameplayTag Attributes_Primary_Mind;
	FGameplayTag Attributes_Primary_Endurance;
	FGameplayTag Attributes_Primary_Dex;
	FGameplayTag Attributes_Primary_Rage;
	FGameplayTag Attributes_Primary_MaxRage;
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

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Toggleable;
	FGameplayTag InputTag_TargetLock;
	FGameplayTag InputTag_Space;
	FGameplayTag InputTag_MustBeHeld;
	FGameplayTag InputTag_MustBeHeld_Block;
	FGameplayTag InputTag_Equip;
	FGameplayTag InputTag_Rage;
	FGameplayTag InputTag_SpecialWeaponAbility_Light;
	FGameplayTag InputTag_SpecialWeaponAbility_Heavy;

	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Magic;
	TArray<FGameplayTag> DamageTypes;
	
	FGameplayTag Effect_HitReact;
	FGameplayTag Effect_Invincible;
	
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	FGameplayTag Abilities_Invisible;
	FGameplayTag Abilities_TargetLock;
	FGameplayTag Abilities_Blocking;
	FGameplayTag Abilities_Weapon;
	FGameplayTag Abilities_Rage;
	FGameplayTag Abilities_SpecialWeaponAbility_Light;
	FGameplayTag Abilities_SpecialWeaponAbility_Heavy;
	FGameplayTag Abilities_KnockDown;
	
	FGameplayTag Enemy_Ability_Melee;
	FGameplayTag Enemy_Status_Strafing;
	FGameplayTag Enemy_Status_KnockedDown;

	FGameplayTag Player_Weapon;
	FGameplayTag Player_Cooldown_SpecialWeaponAbility_Light;
	FGameplayTag Player_Cooldown_SpecialWeaponAbility_Heavy;
	
	FGameplayTag Status_Blocking;
	FGameplayTag Status_Rage_Activating;
	FGameplayTag Status_Rage_Active;
	FGameplayTag Status_Rage_Full;
	FGameplayTag Status_Rage_None;
	
	FGameplayTag Animation_TargetLock;
	
	FGameplayTag Event_Weapon_Impact;
	FGameplayTag Event_SuccessfulBlock;
	FGameplayTag Event_KnockDown;
	FGameplayTag Event_HitPause;
	FGameplayTag Event_MeleeHit;
	FGameplayTag Event_Equip;
	FGameplayTag Event_UnEquip;
	FGameplayTag Event_Activate_Rage;
	FGameplayTag Event_Spawn_Projectile;
	FGameplayTag Event_AOE;
	FGameplayTag Event_HitReact;
	FGameplayTag Event_SaveAttack;
	FGameplayTag Event_ResetCombo;

protected:

private:
	static FEldenGameplayTags GameplayTags;
};
