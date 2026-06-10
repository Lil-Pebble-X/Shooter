// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDataUnderCrosshair.h"
#include "CollisionQueryParams.h"
#include "ShooterCharacter.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"


UTargetDataUnderCrosshair* UTargetDataUnderCrosshair::CreateTargetDataUnderCrosshair(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderCrosshair* MyObj = NewAbilityTask<UTargetDataUnderCrosshair>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderCrosshair::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (!bIsLocallyControlled)
	{
		EndTask();
		return;
	}

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	AShooterCharacter* Character = Cast<AShooterCharacter>(Ability->GetCurrentActorInfo()->AvatarActor.Get());
	if (!PC || !Character) { EndTask(); return; }

	FVector CamLoc;
	FRotator CamRot;
	PC->GetPlayerViewPoint(CamLoc, CamRot);

	const float MaxTraceDistance = 50000.f;
	const FVector End = CamLoc + CamRot.Vector() * MaxTraceDistance;

	FCollisionQueryParams QueryParams(
		SCENE_QUERY_STAT(CameraTrace), 
		false,                          
		Character                       
	);

	FHitResult CameraHit;
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		CameraHit, CamLoc, End,
		ECC_Visibility,
		QueryParams
	);

	FVector TargetPoint = bHit ? CameraHit.ImpactPoint : End;

	FVector MuzzleLoc = Character->GetCombatSocketLocation();
	FVector RealAimDirection = (TargetPoint - MuzzleLoc).GetSafeNormal();

	ValidData.Broadcast(RealAimDirection);
}
