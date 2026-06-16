// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDataUnderCrosshair.h"
#include "ShooterCharacter.h"
#include "AbilitySystemComponent.h"
#include "demo.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilitySpec.h"
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

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	if (bHit)
	{
		Data->HitResult = CameraHit;
	}
	else
	{
		Data->HitResult.bBlockingHit = false;   
		Data->HitResult.Location = TraceEnd;     
		Data->HitResult.ImpactPoint = TraceEnd;  
		Data->HitResult.TraceEnd = TraceEnd;
	}
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
		GetActivationPredictionKey()
	);

	//Helped me solve a long-standing problem, leaving this as a memento.
	/*
	if (DataHandle.Num() > 0)
	{
		const FGameplayAbilityTargetData* First = DataHandle.Get(0);
		if (const FGameplayAbilityTargetData_SingleTargetHit* HitData = static_cast<const FGameplayAbilityTargetData_SingleTargetHit*>(First))
		{
			const FHitResult& HR = HitData->HitResult;
			UE_LOG(LogTemp, Warning, TEXT("[SERVER] Received HitData: bBlockingHit=%d Impact=(%f,%f,%f) Location=(%f,%f,%f) Actor=%s"),
				HR.bBlockingHit,
				HR.ImpactPoint.X, HR.ImpactPoint.Y, HR.ImpactPoint.Z,
				HR.Location.X, HR.Location.Y, HR.Location.Z,
				HR.GetActor() ? *HR.GetActor()->GetName() : TEXT("None")
			);
		}
		else if (const FGameplayAbilityTargetData_LocationInfo* LocData = static_cast<const FGameplayAbilityTargetData_LocationInfo*>(First))
		{
			const FVector Loc = LocData->TargetLocation.GetTargetingTransform().GetLocation();
			UE_LOG(LogTemp, Warning, TEXT("[SERVER] Received LocationData: Location=(%f,%f,%f)"), Loc.X, Loc.Y, Loc.Z);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[SERVER] Received TargetData of unknown type"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[SERVER] Received empty DataHandle"));
	}
	*/

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
