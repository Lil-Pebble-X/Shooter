// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameplayTags.h"
#include "GameplayTagsManager.h"

FShooterGameplayTags FShooterGameplayTags::GameplayTags;

void FShooterGameplayTags::InitializeNativeGameplayTags()
{
	/*
	*   Attributes_Primary_Tags
	*/
	GameplayTags.Attributes_Primary_CritRate = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.CritRate"),
		FString("Chance to deal extra damage on hit")
	);

	GameplayTags.Attributes_Primary_MaxCritRate = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.MaxCritRate"),
		FString("Maximum chance to deal extra damage")
	);

	GameplayTags.Attributes_Primary_ElementalMastery = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.ElementalMastery"),
		FString("Increases corresponding attribute")
	);

	GameplayTags.Attributes_Primary_CritDmg = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.CritDmg"),
		FString("Bonus damage on critical hit")
	);

	GameplayTags.Attributes_Primary_AttackPower = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.AttackPower"),
		FString("Base value of damage dealt")
	);

	GameplayTags.Attributes_Primary_Speed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Speed"),
		FString("Character Movement Speed")
	);

	GameplayTags.Attributes_Primary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.MaxHealth"),
		FString("Maximum amount of Health obtainable")
	);

	GameplayTags.Attributes_Primary_MaxShield = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.MaxShield"),
		FString("Maximum amount of Shield obtainable")
	);


	/*
	*   InputTags
	*/
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button")
	);

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input Tag for Right Mouse Button")
	);

	GameplayTags.InputTag_B = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.B"),
		FString("Input Tag for B Key")
	);

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input Tag for Number 1 Key")
	);

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input Tag for Number 2 Key")
	);

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input Tag for Number 3 Key")
	);

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage")
	);

	/*
	*   DamageTypes
	*/

	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("Fire Damage Type")
	);

	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Lightning"),
		FString("Lightning Damage Type")
	);

	GameplayTags.Damage_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Ice"),
		FString("Ice Damage Type")
	);

	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage Type")
	);

	/*
	*   Resistances
	*/

	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Fire"),
		FString("Resistance to Fire Damage")
	);

	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Lightning"),
		FString("Resistance to Lightning Damage")
	);

	GameplayTags.Attributes_Resistance_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Ice"),
		FString("Resistance to Ice Damage")
	);

	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Physical"),
		FString("Resistance to Physical Damage")
	);

	/*
	*   Battle
	*/

	GameplayTags.Attributes_Battle_ShieldRegenBlock = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Battle.ShieldRegenBlock"),
		FString("Block shield regeneration after taking damage")
	);

	/*
	* Map of DamageTypes to Resistances
	*/

	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Ice, GameplayTags.Attributes_Resistance_Ice);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);


	/*
	* Effects
	*/

	/*
	* Abilities
	*/
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("Attack Ability Tag")
	);
}
