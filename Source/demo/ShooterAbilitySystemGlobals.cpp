// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAbilitySystemGlobals.h"
#include "ShooterAbilityTypes.h"

FGameplayEffectContext* UShooterAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FShooterGameplayEffectContext();
}
