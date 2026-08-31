// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterGameplayAbility.h"
#include "GA_EquipWeaponBase.generated.h"

class USkeletalMesh;

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

	//Provide Fire Ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equip")
	TSubclassOf<UGameplayAbility> FireAbilityToEquip;

	//Bind Mesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equip")
	TObjectPtr<USkeletalMesh> WeaponMesh;

	//Weapon Muzzle Socket Name
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equip")
	FName WeaponTipSocket;
};
