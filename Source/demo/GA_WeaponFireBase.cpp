// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_WeaponFireBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ShooterGameplayAbility.h"
#include "ShooterProjectile.h"
#include "CombatInterface.h"
#include "ShooterPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ShooterGameplayTags.h"

void UGA_WeaponFireBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
										  const FGameplayAbilityActorInfo* ActorInfo,
										  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}
