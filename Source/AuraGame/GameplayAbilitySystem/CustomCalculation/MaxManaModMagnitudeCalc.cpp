
#include "MaxManaModMagnitudeCalc.h"

#include "AuraGame/GameplayAbilitySystem/AuraAttributeSet.h"
#include "AuraGame/Interaction/CombatInterface.h"

UMaxManaModMagnitudeCalc::UMaxManaModMagnitudeCalc()
{
	IntelligenceDefinition.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDefinition);
}

float UMaxManaModMagnitudeCalc::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float Intelligence = 0.0f;
	GetCapturedAttributeMagnitude(IntelligenceDefinition, Spec, EvaluateParams, Intelligence);
	Intelligence = FMath::Max<float>(Intelligence, 0.0f); // Ensure Intelligence is non-negative
	const int32 PlayerLevel = ICombatInterface::Execute_IGetCharacterLevel(Spec.GetContext().GetSourceObject());

	return 50.f + (Intelligence * 2.5f) + (PlayerLevel * 15.f);
}
