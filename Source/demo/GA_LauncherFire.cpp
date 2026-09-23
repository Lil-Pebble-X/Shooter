// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_LauncherFire.h"
#include "CombatInterface.h"
#include "ShooterGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ShooterProjectile.h"
#include "AbilitySystemComponent.h"

UGA_LauncherFire::UGA_LauncherFire()
{
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_LauncherFire::OnServerFireResolved(const FHitResult& HitResult)
{
	SpawnProjectile(HitResult.Location);
}

void UGA_LauncherFire::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	if (!ProjectileClass) return;
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo());
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		AShooterProjectile* Projectile = GetWorld()->SpawnActorDeferred<AShooterProjectile>(
			ProjectileClass, SpawnTransform,
			GetAvatarActorFromActorInfo(), 
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.SetAbility(this);
		EffectContextHandle.AddSourceObject(Projectile);
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors(Actors);
		FHitResult HitResult;
		HitResult.Location = ProjectileTargetLocation;
		EffectContextHandle.AddHitResult(HitResult);

		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

		for (auto& Pair : DamageTypes)
		{
			const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
		}

		FGameplayEffectSpecHandle StatusSpecHandle;
		if (OnHitStatusEffect)
		{
			StatusSpecHandle = SourceASC->MakeOutgoingSpec(OnHitStatusEffect, GetAbilityLevel(), EffectContextHandle);
		}

		Projectile->DamageEffectSpecHandle = SpecHandle;
		Projectile->StatusEffectSpecHandle = StatusSpecHandle;

		Projectile->FinishSpawning(SpawnTransform);

	}
}