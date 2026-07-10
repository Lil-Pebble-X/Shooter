// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.h"
#include "ShooterGameplayTags.h"

struct ShooterDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Shield);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritRate);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDmg);

	ShooterDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Shield, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CritRate, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CritDmg, Target, false);
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
	RelevantAttributesToCapture.Add(DamageStatics().CritRateDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritDmgDef);
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

	// Get Damage Set by Caller Magnitude
	float Damage = Spec.GetSetByCallerMagnitude(FShooterGameplayTags::Get().Damage);

	// Check critical hit chance and critical damage multiplier; determine if a critical hit occurs.
	// If critical, increase damage by the critical damage multiplier.

	float TargetCritRate = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritRateDef, EvaluationParameters, TargetCritRate);
	TargetCritRate = FMath::Max<float>(TargetCritRate, 0.f);

	float TargetCritDmg = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritDmgDef, EvaluationParameters, TargetCritDmg);
	TargetCritDmg = FMath::Max<float>(TargetCritDmg, 0.f);

	const bool bCriticalHit = FMath::RandRange(0, 99) <TargetCritRate;
	if (bCriticalHit)
	{
		Damage = (TargetCritDmg / 100.f) * Damage;
	}

	// Detect Shield; if present, absorb damage with the shield.


	const FGameplayModifierEvaluatedData EvaluatedDta(UBaseAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedDta);
}
