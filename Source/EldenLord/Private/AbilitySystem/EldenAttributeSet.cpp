// luce copyright


#include "AbilitySystem/EldenAttributeSet.h"
#include "Net/UnrealNetwork.h"

UEldenAttributeSet::UEldenAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitMana(100.f);
	InitMaxMana(100.f);
	InitStamina(100.f);
	InitMaxStamina(100.f);
}

void UEldenAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEldenAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
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
