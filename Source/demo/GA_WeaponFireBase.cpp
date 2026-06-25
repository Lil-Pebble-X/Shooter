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

void UGA_WeaponFireBase::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		AShooterProjectile* Projectile = GetWorld()->SpawnActorDeferred<AShooterProjectile>(
			ProjectileClass, SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		const UAbilitySystemComponent* SourceASC =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		
		FShooterGameplayTags GameplayTags = FShooterGameplayTags::Get();
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, 30.f);

		Projectile->DamageEffectSpecHandle = SpecHandle;

		Projectile->FinishSpawning(SpawnTransform);
	}
}

bool UGA_WeaponFireBase::PerformHitscan(FHitResult& OutHit)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return false;	

	const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
	if (!ActorInfo || !ActorInfo->PlayerController.IsValid())
	{
		return false;
	}

	APlayerController* PC = ActorInfo->PlayerController.Get();
	if (!PC) return false;

	FVector Start;
	FRotator Rot;
	PC->GetPlayerViewPoint(Start, Rot);

	const FVector End = Start + Rot.Vector() * HitscanRange;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetAvatarActorFromActorInfo());

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		OutHit, Start, End, HitscanTraceChannel, QueryParams
	);
	return bHit;
}
