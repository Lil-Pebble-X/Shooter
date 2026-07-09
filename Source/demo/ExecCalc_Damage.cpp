// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.h"

struct ShooterDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Shield);

	ShooterDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Shield, Target, false);
	}
};

static const ShooterDamageStatics& DamageStatics()
{
	static ShooterDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ShieldDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Shield = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ShieldDef,	EvaluationParameters, Shield);
	Shield = FMath::Max<float>(0.f, Shield);
	--Shield;

	const FGameplayModifierEvaluatedData EvaluatedDta(DamageStatics().ShieldProperty, EGameplayModOp::Additive, Shield);
	OutExecutionOutput.AddOutputModifier(EvaluatedDta);
}
