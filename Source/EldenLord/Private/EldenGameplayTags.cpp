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

	/*
	 *  Status Tags
	 */
	
	GameplayTags.Status_Blocking = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Status.Blocking"),
		FString("Status Block Ability")
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

	GameplayTags.Event_HitPause = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.HitPause"),
		FString("HitPause Event")
	);

	GameplayTags.Event_MeleeHit = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.MeleeHit"),
		FString("Melee Hit Event")
	);
}
