// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EldenGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ELDENLORD_API UEldenGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartUpInputTag;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage")
	FScalableFloat Damage;
};
