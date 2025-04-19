// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxRage.generated.h"

/**
 * 
 */
UCLASS()
class ELDENLORD_API UMMC_MaxRage : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MaxRage();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
private:

	FGameplayEffectAttributeCaptureDefinition DexDef;
};
