// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "PawnCombatComponent.h"
#include "Components/PawnExtentionComponentBase.h"
#include "EnemyCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class ELDENLORD_API UEnemyCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
public:
	virtual void OnHitTargetActor(AActor* HitActor) override;
};
