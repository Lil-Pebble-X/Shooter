// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_EquipWeaponBase.h"
#include "ShooterCharacter.h"

UGA_EquipWeaponBase::UGA_EquipWeaponBase()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_EquipWeaponBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AShooterCharacter* Character = Cast<AShooterCharacter>(GetAvatarActorFromActorInfo()))
	{
		Character->EquipWeapon(FireAbilityToEquip, WeaponMesh, WeaponTipSocket);
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}