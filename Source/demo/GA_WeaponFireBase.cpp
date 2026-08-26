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
#include "TargetDataUnderCrosshair.h"

void UGA_WeaponFireBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
										  const FGameplayAbilityActorInfo* ActorInfo,
										  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	StartTargeting();

}

void UGA_WeaponFireBase::StartTargeting()
{
	UTargetDataUnderCrosshair* Task = UTargetDataUnderCrosshair::CreateTargetDataUnderCrosshair(this);
	Task->ValidData.AddDynamic(this, &UGA_WeaponFireBase::OnTargetDataReady);
	Task->ReadyForActivation();
}

void UGA_WeaponFireBase::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	const auto* Data = static_cast<const FGameplayAbilityTargetData_SingleTargetHit*>(DataHandle.Get(0));
	if (!Data) return;

	const FHitResult HitResult = Data->HitResult;

	OnLocalFireEffects(HitResult);                 

	if (HasAuthority(&CurrentActivationInfo))     
	{
		if (ValidateTarget(HitResult))
		{
			OnServerFireResolved(HitResult);      
		}
	}
}

void UGA_WeaponFireBase::OnLocalFireEffects(const FHitResult& HitResult)
{
}

void UGA_WeaponFireBase::OnServerFireResolved(const FHitResult& HitResult)
{
}

bool UGA_WeaponFireBase::ValidateTarget(const FHitResult& HitResult) const
{
	return true;
}

FGameplayEffectSpecHandle UGA_WeaponFireBase::BuildDamageSpec(const FHitResult& HitResult)
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	if (!SourceASC) return FGameplayEffectSpecHandle();

	FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddHitResult(HitResult);

	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ContextHandle);

	for (const auto& Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
	}

	return SpecHandle;
}

