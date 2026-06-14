// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDataUnderCrosshair.h"
#include "ShooterCharacter.h"
#include "AbilitySystemComponent.h"
#include "demo.h"
#include "Abilities/GameplayAbilityTargetTypes.h"

UTargetDataUnderCrosshair* UTargetDataUnderCrosshair::CreateTargetDataUnderCrosshair(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderCrosshair* MyObj = NewAbilityTask<UTargetDataUnderCrosshair>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderCrosshair::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendCrosshairTargetData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();

		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(
			GetAbilitySpecHandle(), GetActivationPredictionKey()
		).AddUObject(this, &UTargetDataUnderCrosshair::OnTargetDataReplicatedCallback);

		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderCrosshair::SendCrosshairTargetData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	AShooterCharacter* Character = Cast<AShooterCharacter>(Ability->GetCurrentActorInfo()->AvatarActor.Get());
	if (!PC || !Character) { EndTask(); return; }

	FVector CamLoc;
	FRotator CamRot;
	PC->GetPlayerViewPoint(CamLoc, CamRot);

	const float MaxTraceDistance = 50000.f;
	const FVector TraceEnd = CamLoc + CamRot.Vector() * MaxTraceDistance;

	FCollisionQueryParams QueryParams(
		SCENE_QUERY_STAT(CameraTrace),
		false,
		Character
	);

	FHitResult CameraHit;
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		CameraHit, CamLoc, TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();

	if (bHit)
	{
		Data->HitResult = CameraHit;
	}
	else
	{
		Data->HitResult.TraceStart = CamLoc;
		Data->HitResult.TraceEnd = TraceEnd;
		Data->HitResult.Location = TraceEnd;
		Data->HitResult.ImpactPoint = TraceEnd;
		Data->HitResult.Normal = -CamRot.Vector();
		Data->HitResult.bBlockingHit = false;
	}

	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderCrosshair::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
