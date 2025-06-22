// luce copyright


#include "AbilitySystem/MMC/MMC_MaxStamina.h"

#include "AbilitySystem/EldenAttributeSet.h"
#include "Interface/CombatInterface.h"

UMMC_MaxStamina::UMMC_MaxStamina()
{
	EnduranceDef.AttributeToCapture = UEldenAttributeSet::GetEnduranceAttribute();
	EnduranceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	EnduranceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(EnduranceDef);
}

float UMMC_MaxStamina::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float Endurance = 0.f;
	GetCapturedAttributeMagnitude(EnduranceDef, Spec, EvaluateParameters,Endurance);
	Endurance = FMath::Max<float>(Endurance,0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
	return 70.f + 1.0f * Endurance + 2.5f * PlayerLevel;
}
