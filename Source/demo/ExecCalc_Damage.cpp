// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.h"
#include "ShooterGameplayTags.h"
#include "CombatInterface.h"
#include "ShooterAbilityTypes.h"
#include "ShooterAbilitySystemLibrary.h"

struct ShooterDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Shield);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritRate);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDmg);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);

	ShooterDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Shield, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CritRate, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CritDmg, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, AttackPower, Source, false);
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
	RelevantAttributesToCapture.Add(DamageStatics().AttackPowerDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Get Damage Set by Caller Magnitude
	float Damage = 0.f;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FShooterGameplayTags::Get().DamageTypesToResistances)
	{
		const float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);
		Damage += DamageTypeValue;
	}

	// Check critical hit chance and critical damage multiplier; determine if a critical hit occurs.
	// If critical, increase damage by the critical damage multiplier.

	float SourceCritRate = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritRateDef, EvaluationParameters, SourceCritRate);
	SourceCritRate = FMath::Max<float>(SourceCritRate, 0.f);

	float SourceCritDmg = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritDmgDef, EvaluationParameters, SourceCritDmg);
	SourceCritDmg = FMath::Max<float>(SourceCritDmg, 0.f);

	const bool bCriticalHit = FMath::RandRange(0, 99) < SourceCritRate;

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UShooterAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	if (bCriticalHit)
	{
		Damage = Damage * (SourceCritDmg * 0.01f);
	}

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPowerDef, EvaluationParameters, SourceAttackPower);
	SourceAttackPower = FMath::Max<float>(SourceAttackPower, 0.f);
	Damage = Damage * (SourceAttackPower * 0.01f);


	const FGameplayModifierEvaluatedData EvaluatedDta(UBaseAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedDta);
}
