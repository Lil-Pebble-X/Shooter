// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterGameplayAbility.h"
#include "GA_EquipWeaponBase.generated.h"

/**
 * 
 */

UCLASS()
class DEMO_API UGA_EquipWeaponBase : public UShooterGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_EquipWeaponBase();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//0 = Primary, 1 = Secondary
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equip")
	int32 SlotIndex = 0;
};
