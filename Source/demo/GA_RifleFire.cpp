// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_RifleFire.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CombatInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "TargetDataUnderCrosshair.h"


UGA_RifleFire::UGA_RifleFire()
{
    NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_RifleFire::FireHitscan()
{
	UTargetDataUnderCrosshair* Task = UTargetDataUnderCrosshair::CreateTargetDataUnderCrosshair(this);
	Task->ValidData.AddDynamic(this, &UGA_RifleFire::OnTargetDataReady);
}

void UGA_RifleFire::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	FHitResult HitResult;
	if (const FGameplayAbilityTargetData_SingleTargetHit* Data =
		static_cast<const FGameplayAbilityTargetData_SingleTargetHit*>(DataHandle.Get(0)))
	{
		HitResult = Data->HitResult;
	}

	PlayFireEffects(HitResult);

	if (GetAvatarActorFromActorInfo()->HasAuthority() && HitResult.GetActor())
	{
		ApplyDamageToTarget(HitResult.GetActor(), HitResult);
	}
}

void UGA_RifleFire::ApplyDamageToTarget(AActor* TargetActor, const FHitResult& HitResult)
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!SourceASC || !TargetASC) return;

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

	SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
}

void UGA_RifleFire::PlayFireEffects(const FHitResult& HitResult)
{
	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	const bool bIsLocalPredicted = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted);
	const bool bShouldPlayFX = bIsLocalPredicted
		? IsLocallyControlled()
		: (ActorInfo && ActorInfo->AvatarActor.IsValid() && ActorInfo->AvatarActor->HasAuthority());

	if (!bShouldPlayFX) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	FVector MuzzleLocation = HitResult.Location;
	if (CombatInterface)
	{
		MuzzleLocation = FVector(CombatInterface->GetCombatSocketLocation());
	}

	const FVector TraceDirection = (HitResult.Location - MuzzleLocation).GetSafeNormal();
	const FRotator TracerRotation = TraceDirection.Rotation();

	if (ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactEffect, HitResult.Location);
	}
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.Location);
	}
	if (TracerEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TracerEffect, MuzzleLocation, TracerRotation);
	}
}
