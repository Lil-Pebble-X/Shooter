// Fill out your copyright notice in the Description page of Project Settings.

#include "MMC_MaxHealth.h"
#include "ShooterPlayerState.h"
#include "CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{

}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());

	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 90.f + 10.f * PlayerLevel;
}
