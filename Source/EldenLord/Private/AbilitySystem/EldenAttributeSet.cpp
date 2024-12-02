// luce copyright


#include "AbilitySystem/EldenAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EldenGameplayTags.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UEldenAttributeSet::UEldenAttributeSet()
{
	const FEldenGameplayTags& GameplayTags = FEldenGameplayTags::Get();

	// Primary Attribute
	TagToAttribute.Add(GameplayTags.Attributes_Primary_Vigor,GetVigorAttribute);
	TagToAttribute.Add(GameplayTags.Attributes_Primary_Mind,GetMindAttribute);
	TagToAttribute.Add(GameplayTags.Attributes_Primary_Endurance,GetEnduranceAttribute);
	TagToAttribute.Add(GameplayTags.Attributes_Primary_Strength,GetStrengthAttribute);
	TagToAttribute.Add(GameplayTags.Attributes_Primary_Intelligence,GetIntelligenceAttribute);
	TagToAttribute.Add(GameplayTags.Attributes_Primary_Resilience,GetResilienceAttribute);
	
	// Secondary Attribute
	TagToAttribute.Add(GameplayTags.Attributes_Secondary_MaxHealth,GetMaxHealthAttribute);
	TagToAttribute.Add(GameplayTags.Attributes_Secondary_MaxMana,GetMaxManaAttribute);
	TagToAttribute.Add(GameplayTags.Attributes_Secondary_MaxStamina,GetMaxStaminaAttribute);
	TagToAttribute.Add(GameplayTags.Attributes_Secondary_Armor,GetArmorAttribute);
	TagToAttribute.Add(GameplayTags.Attributes_Secondary_ArmorPenetration,GetArmorPenetrationAttribute);
	TagToAttribute.Add(GameplayTags.Attributes_Secondary_BlockChance,GetBlockChanceAttribute);
	TagToAttribute.Add(GameplayTags.Attributes_Secondary_CriticalHitChance,GetCriticalHitChanceAttribute);
	TagToAttribute.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage,GetCriticalHitDamageAttribute);
	TagToAttribute.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance,GetCriticalHitResistanceAttribute);
}

void UEldenAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Primary Attribute

	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, Mind, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, Endurance, COND_None, REPNOTIFY_Always);
	
	// Secondary Attribute

	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);

	// Vital Attribute

	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
}

void UEldenAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
}

void UEldenAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source = causer of the effect, Target = target of the effect (owner of this AS)

	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->
		AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			ACharacter* SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UEldenAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);
}

void UEldenAttributeSet::OnRep_Health(const FGameplayAttributeData OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEldenAttributeSet, Health, OldHealth);
}

void UEldenAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEldenAttributeSet, MaxHealth, OldMaxHealth)
}

void UEldenAttributeSet::OnRep_Mana(const FGameplayAttributeData OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEldenAttributeSet, Mana, OldMana)
}

void UEldenAttributeSet::OnRep_MaxMana(const FGameplayAttributeData OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEldenAttributeSet, MaxMana, OldMaxMana)
}

void UEldenAttributeSet::OnRep_Stamina(const FGameplayAttributeData OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEldenAttributeSet, Stamina, OldStamina)
}

void UEldenAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEldenAttributeSet, MaxStamina, OldMaxStamina)
}

void UEldenAttributeSet::OnRep_Strength(const FGameplayAttributeData OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEldenAttributeSet, Strength, OldStrength)
}

void UEldenAttributeSet::OnRep_Intelligence(const FGameplayAttributeData OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEldenAttributeSet, Intelligence, OldIntelligence)
}

void UEldenAttributeSet::OnRep_Resilience(const FGameplayAttributeData OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEldenAttributeSet, Resilience, OldResilience)
}

void UEldenAttributeSet::OnRep_Vigor(const FGameplayAttributeData OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEldenAttributeSet, Vigor, OldVigor)
}

void UEldenAttributeSet::OnRep_Mind(const FGameplayAttributeData OldMind) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEldenAttributeSet, Mind, OldMind)
}

void UEldenAttributeSet::OnRep_Endurance(const FGameplayAttributeData OldEndurance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEldenAttributeSet, Endurance, OldEndurance)
}

void UEldenAttributeSet::OnRep_Armor(const FGameplayAttributeData OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEldenAttributeSet, Armor, OldArmor)
}

void UEldenAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEldenAttributeSet, ArmorPenetration, OldArmorPenetration)
}

void UEldenAttributeSet::OnRep_BlockChance(const FGameplayAttributeData OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEldenAttributeSet, BlockChance, OldBlockChance)
}

void UEldenAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEldenAttributeSet, CriticalHitChance, OldCriticalHitChance)
}

void UEldenAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEldenAttributeSet, CriticalHitDamage, OldCriticalHitDamage)
}

void UEldenAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEldenAttributeSet, CriticalHitResistance, OldCriticalHitResistance)
}

void UEldenAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEldenAttributeSet, HealthRegeneration, OldHealthRegeneration)
}
