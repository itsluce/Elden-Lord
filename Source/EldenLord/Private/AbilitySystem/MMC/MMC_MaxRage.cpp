// luce copyright


#include "AbilitySystem/MMC/MMC_MaxRage.h"

#include "AbilitySystem/EldenAttributeSet.h"
#include "Interface/CombatInterface.h"

UMMC_MaxRage::UMMC_MaxRage()
{
	DexDef.AttributeToCapture = UEldenAttributeSet::GetDexAttribute();
	DexDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	DexDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(DexDef);
}

float UMMC_MaxRage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float Dex = 0.f;
	GetCapturedAttributeMagnitude(DexDef, Spec, EvaluateParameters, Dex);
	Dex = FMath::Max<float>(Dex, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
	return 50.f + 2.5f * Dex + 10.f * PlayerLevel;
}
