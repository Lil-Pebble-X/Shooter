// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_RifleFire.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CombatInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "TargetDataUnderCrosshair.h"
#include "DrawDebugHelpers.h"


UGA_RifleFire::UGA_RifleFire()
{
    NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UGA_RifleFire::ValidateTarget(const FHitResult& HitResult) const
{
	return HitResult.bBlockingHit
		&& HitResult.GetActor() != nullptr
		&& HitResult.GetActor() != GetAvatarActorFromActorInfo();
}

void UGA_RifleFire::OnLocalFireEffects(const FHitResult& HitResult)
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	FVector MuzzleLocation = HitResult.Location;
	if (CombatInterface)
	{
		MuzzleLocation = FVector(CombatInterface->GetCombatSocketLocation());
	}

	const FVector TraceDirection = (HitResult.Location - MuzzleLocation).GetSafeNormal();
	const FRotator TracerRotation = TraceDirection.Rotation();

	if (ImpactEffect)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactEffect, HitResult.Location);
	if (ImpactSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.Location);
	if (TracerEffect)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TracerEffect, MuzzleLocation, TracerRotation);
}

void UGA_RifleFire::OnServerFireResolved(const FHitResult& HitResult)
{
	ApplyDamageToTarget(HitResult.GetActor(), HitResult);
}

void UGA_RifleFire::ApplyDamageToTarget(AActor* TargetActor, const FHitResult& HitResult)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC) return;

	FGameplayEffectSpecHandle SpecHandle = BuildDamageSpec(HitResult);
	if (!SpecHandle.IsValid()) return;

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
}

