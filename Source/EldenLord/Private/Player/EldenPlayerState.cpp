// luce copyright


#include "Player/EldenPlayerState.h"

#include "AbilitySystem/EldenAbilitySystemComponent.h"
#include "AbilitySystem/EldenAttributeSet.h"

AEldenPlayerState::AEldenPlayerState()
{

	AbilitySystemComponent = CreateDefaultSubobject<UEldenAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UEldenAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AEldenPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
