// luce copyright


#include "EldenGameplayTags.h"
#include "GameplayTagsManager.h"

FEldenGameplayTags FEldenGameplayTags::GameplayTags;

void FEldenGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * Primary Attributes
	 */

	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("Increases Health")
	);

	GameplayTags.Attributes_Primary_Mind = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Mind"),
		FString("Increases Mana")
	);

	GameplayTags.Attributes_Primary_Endurance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Endurance"),
		FString("Increases Stamina")
	);

	GameplayTags.Attributes_Primary_Dex = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Dex"),
		FString("Increases Dex")
	);

	GameplayTags.Attributes_Primary_Rage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Rage"),
		FString("Increases Rage")
	);

	GameplayTags.Attributes_Primary_MaxRage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.MaxRage"),
		FString("Increases Max Rage")
	);

	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increases physical damage")
	);

	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increases magical damage")
	);

	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("Increases Armor and Armor Penetration")
	);


	/*
	 * Secondary Attributes
	 */

	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces damage taken, improves Block Chance")
	);

	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"),
		FString("Ignores Percentage of enemy Armor, increases Critical Hit Chance")
	);

	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("Chance to cut incoming damage in half")
	);

	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("Chance to double damage plus critical hit bonus")
	);

	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Bonus damage added when a critical hit is scored")
	);

	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"),
		FString("Reduces Critical Hit Chance of attacking enemies")
	);

	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum amount of Health obtainable")
	);

	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("Maximum amount of Mana obtainable")
	);

	GameplayTags.Attributes_Secondary_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxStamina"),
		FString("Maximum amount of Stamina obtainable")
	);

	/*
	 * Input Tags
	 */

	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button")
	);

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input Tag for Right Mouse Button")
	);

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input Tag for 1 Key")
	);

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input Tag for 2 Key")
	);

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input Tag for 3 Key")
	);

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input Tag for 4 Key")
	);

	GameplayTags.InputTag_TargetLock = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.TargetLock"),
		FString("Input Tag for Target Lock")
	);
	GameplayTags.InputTag_Toggleable = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Toggleable"),
		FString("Input Tag for Toggleable")
	);
	GameplayTags.InputTag_Space = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Space"),
		FString("Input Tag for Dodge")
	);

	GameplayTags.InputTag_MustBeHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.MustBeHeld"),
		FString("Input Tag for Block")
	);

	GameplayTags.InputTag_MustBeHeld_Block = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.MustBeHeld_Block"),
		FString("Input Tag for Block")
	);

	GameplayTags.InputTag_Equip = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Equip"),
		FString("Input Tag for Equip Weapon")
	);

	GameplayTags.InputTag_Rage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Rage"),
		FString("Input Tag for Rage")
	);

	GameplayTags.InputTag_SpecialWeaponAbility_Light = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.SpecialWeaponAbility.Light"),
		FString("Input Tag for Special Weapon Ability Light Attack")
	);

	GameplayTags.InputTag_SpecialWeaponAbility_Heavy = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.SpecialWeaponAbility.Heavy"),
		FString("Input Tag for Special Weapon Ability Heavy Attack")
	);

	/*
	 * Damage Tags
	 */

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage")
	);

	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("Fire Damage")
	);

	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage")
	);

	GameplayTags.Damage_Magic = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Magic"),
		FString("Magic Damage")
	);

	GameplayTags.DamageTypes.Add(GameplayTags.Damage);

	/*
     *  Effect Tags
     */

	GameplayTags.Effect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effect.HitReact"),
		FString("Effect HitReact")
	);
	GameplayTags.Effect_Invincible = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effect.Invincible"),
		FString("Effect Invincible")
	);

	/*
     *  Abilities Tags
     */

	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("Attack Ability")
	);

	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Summon"),
		FString("Summon Ability")
	);

	GameplayTags.Abilities_Invisible = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Invisible"),
		FString("Invisible Ability")
	);

	GameplayTags.Abilities_TargetLock = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.TargetLock"),
		FString("Target Lock Ability")
	);

	GameplayTags.Abilities_Blocking = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Blocking"),
		FString("Block Ability")
	);

	GameplayTags.Abilities_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Weapon"),
		FString("Weapon Ability")
	);

	GameplayTags.Abilities_Rage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Rage"),
		FString("Rage Ability")
	);

	GameplayTags.Abilities_SpecialWeaponAbility_Light = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.SpecialWeaponAbility.Light"),
		FString("Special WeaponAbility Light Attack Ability")
	);

	GameplayTags.Abilities_SpecialWeaponAbility_Heavy = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.SpecialWeaponAbility.Heavy"),
		FString("Special WeaponAbility Heavy Attack Ability")
	);
	
	GameplayTags.Abilities_KnockDown = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.KnockDown"),
		FString("Knock Down Ability")
	);

	/*
	*  Enemy Tags
	*/

	GameplayTags.Enemy_Ability_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Enemy.Ability.Melee"),
		FString("Enemy Melee Attack")
	);

	GameplayTags.Enemy_Status_Strafing = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Enemy.Status.Strafing"),
		FString("Enemy Status Strafing")
	);

	GameplayTags.Enemy_Status_KnockedDown = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Enemy.Status.KnockedDown"),
		FString("Enemy Status Knocked Down")
	);

	/*
	 *  Status Tags
	 */

	GameplayTags.Status_Blocking = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Blocking"),
		FString("Status Block Ability")
	);

	GameplayTags.Status_Rage_Activating = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Rage.Activating"),
		FString("Status Rage Activating Ability")
	);

	GameplayTags.Status_Rage_Active = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Rage.Active"),
		FString("Status Rage Activate Ability")
	);

	GameplayTags.Status_Rage_Full = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Rage.Full"),
		FString("Status Rage Full Ability")
	);

	GameplayTags.Status_Rage_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Rage.None"),
		FString("Status Rage None Ability")
	);

	GameplayTags.Player_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Weapon"),
		FString("Player Weapon")
	);

	GameplayTags.Player_Cooldown_SpecialWeaponAbility_Light = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Cooldown.SpecialWeaponAbility.Light"),
		FString("Player Special Weapon Ability Light Cooldown")
	);

	GameplayTags.Player_Cooldown_SpecialWeaponAbility_Heavy = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Cooldown.SpecialWeaponAbility.Heavy"),
		FString("Player Special Weapon Ability Heavy Cooldown")
	);

	/*
	 *  Animation Tags
	 */

	GameplayTags.Animation_TargetLock = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Animation.TargetLock"),
		FString("Target Lock Animation")
	);

	/*
	 *  Event Tags
	 */

	GameplayTags.Event_Weapon_Impact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Weapon.Impact"),
		FString("Weapon Impact")
	);

	GameplayTags.Event_SuccessfulBlock = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.SuccessfulBlock"),
		FString("Block Event")
	);

	GameplayTags.Event_KnockDown = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.KnockDown"),
		FString("Knock Down Event")
	);

	GameplayTags.Event_HitPause = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.HitPause"),
		FString("HitPause Event")
	);

	GameplayTags.Event_MeleeHit = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.MeleeHit"),
		FString("Melee Hit Event")
	);

	GameplayTags.Event_Equip = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Equip"),
		FString("Equip Weapon Event")
	);

	GameplayTags.Event_UnEquip = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.UnEquip"),
		FString("UnEquip Weapon Event")
	);

	GameplayTags.Event_Activate_Rage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Activate.Rage"),
		FString("Activate Rage Event")
	);

	GameplayTags.Event_Spawn_Projectile = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Spawn.Projectile"),
		FString("Spawn Projectile Event")
	);

	GameplayTags.Event_AOE = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.AOE"),
		FString("AOE Attack Event")
	);

	GameplayTags.Event_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.HitReact"),
		FString("Hit React Event")
	);

	GameplayTags.Event_SaveAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.SaveAttack"),
		FString("Save Attack Event")
	);

	GameplayTags.Event_ResetCombo = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.ResetCombo"),
		FString("Reset Combo Event")
	);
}
