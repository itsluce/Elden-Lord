// luce copyright


#include "Player/EldenPlayerState.h"

#include "AbilitySystem/EldenAbilitySystemComponent.h"
#include "AbilitySystem/EldenAttributeSet.h"
#include "Net/UnrealNetwork.h"

AEldenPlayerState::AEldenPlayerState()
{

	AbilitySystemComponent = CreateDefaultSubobject<UEldenAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UEldenAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;
}

void AEldenPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEldenPlayerState, Level);
}

UAbilitySystemComponent* AEldenPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEldenPlayerState::OnRep_Level(const int32 OldLevel) const
{
}
