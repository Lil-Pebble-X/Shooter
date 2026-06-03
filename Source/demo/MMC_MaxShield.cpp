// Fill out your copyright notice in the Description page of Project Settings.

#include "MMC_MaxShield.h"
#include "ShooterPlayerState.h"
#include "CombatInterface.h"

UMMC_MaxShield::UMMC_MaxShield()
{
}

float UMMC_MaxShield::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());

	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 45.f + 5.f * PlayerLevel;
}

