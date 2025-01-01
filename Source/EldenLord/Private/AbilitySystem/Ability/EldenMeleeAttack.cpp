// luce copyright


#include "AbilitySystem/Ability/EldenMeleeAttack.h"

void UEldenMeleeAttack::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	ICombatInterface::GetHit_Implementation(ImpactPoint, Hitter);
}
