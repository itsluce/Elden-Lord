// luce copyright


#include "AbilitySystem/MMC/MMC_MaxMana.h"

#include "AbilitySystem/EldenAttributeSet.h"
#include "Interface/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	MindDef.AttributeToCapture = UEldenAttributeSet::GetMindAttribute();
	MindDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MindDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(MindDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float Mind = 0.f;
	GetCapturedAttributeMagnitude(MindDef, Spec, EvaluateParameters, Mind);
	Mind = FMath::Max<float>(Mind, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 50.f + 2.0f * Mind + 5.f * PlayerLevel;
}
