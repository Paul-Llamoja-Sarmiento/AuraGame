
#include "MaxHealthModMagnitudeCalc.h"

#include "AuraGame/GameplayAbilitySystem/AuraAttributeSet.h"
#include "AuraGame/Interaction/CombatInterface.h"

UMaxHealthModMagnitudeCalc::UMaxHealthModMagnitudeCalc()
{
	VigorDefinition.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDefinition);
}

float UMaxHealthModMagnitudeCalc::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float Vigor = 0.0f;
	GetCapturedAttributeMagnitude(VigorDefinition, Spec, EvaluateParams, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.0f); // Ensure Vigor is non-negative
	const int32 PlayerLevel = ICombatInterface::Execute_IGetCharacterLevel(Spec.GetContext().GetSourceObject());

	return 80.f + (Vigor * 2.5f) + (PlayerLevel * 10.f);
}
