// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * ShooterGameplayTags
 * 
 * Singleton containing native Gameplay Tags
 */

struct FShooterGameplayTags
{
public:

	static const FShooterGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Primary_CritRate;
	FGameplayTag Attributes_Primary_MaxCritRate;
	FGameplayTag Attributes_Primary_ElementalMastery;
	FGameplayTag Attributes_Primary_CritDmg;
	FGameplayTag Attributes_Primary_AttackPower;
	FGameplayTag Attributes_Primary_Speed;
	FGameplayTag Attributes_Primary_MaxHealth;
	FGameplayTag Attributes_Primary_MaxShield;

	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Ice;
	FGameplayTag Attributes_Resistance_Physical;

	FGameplayTag Attributes_Battle_ShieldRegenBlock;


	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_B;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;

	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Ice;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Physical;

	FGameplayTag Abilities_Attack;

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

private:
	 
	static FShooterGameplayTags GameplayTags;

};