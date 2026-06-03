// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackNotifyState.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Engine/OverlapResult.h"
#include "BaseCharacter.h"
#include "Engine/DamageEvents.h"

UAttackNotifyState::UAttackNotifyState()
{
	DetectRadius = 100.0f;
	DamageAmount = 20.0f;
	AngleLimit = 90.0f;
}

void UAttackNotifyState::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp)
	{
		PerformDamageCheck(MeshComp);
	}
}

void UAttackNotifyState::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

void UAttackNotifyState::PerformDamageCheck(USkeletalMeshComponent* MeshComp)
{
	if (!MeshComp || !MeshComp->GetOwner()) return;

	AActor* OwnerActor = MeshComp->GetOwner();
	UWorld* World = OwnerActor->GetWorld();
	if (!World) return;

	ABaseCharacter* AttackerCharacter = Cast<ABaseCharacter>(OwnerActor);
	if (!AttackerCharacter)
	{
		return;
	}
	float Damage = AttackerCharacter ? AttackerCharacter->Damage : DamageAmount;

	FVector OwnerLocation = OwnerActor->GetActorLocation();
	FVector StartLocation = OwnerLocation + FVector(0, 0, 50.0f);

	FCollisionShape SphereShape = FCollisionShape::MakeSphere(DetectRadius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerActor);

	TArray<FOverlapResult> Overlaps;

	bool bHit = World->OverlapMultiByChannel(
		Overlaps,
		StartLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		SphereShape,
		Params
	);

	//DrawDebugSphere(World, StartLocation, DetectRadius, 16, FColor::Green, false, 0.3f, 0, 1.0f);

	if (bHit)
	{
		const float CosAngleThreshold = FMath::Cos(FMath::DegreesToRadians(AngleLimit));
		const FVector OwnerForwardVector = OwnerActor->GetActorForwardVector();
		for (const FOverlapResult& Overlap : Overlaps)
		{
			AActor* HitActor = Overlap.GetActor();
			if (!HitActor) continue;
			if (IgnoreActorClass && HitActor->IsA(IgnoreActorClass))
			{
				continue;
			}
			FVector ToTarget = HitActor->GetActorLocation() - OwnerLocation;
			ToTarget.Z = 0.0f;
			ToTarget = ToTarget.GetSafeNormal();

			const float DotProduct = FVector::DotProduct(OwnerForwardVector, ToTarget);
			if (DotProduct >= CosAngleThreshold)
			{
				if (ABaseCharacter* TargetCharacter = Cast<ABaseCharacter>(HitActor))
				{
					FDamageEvent DamageEvent;
					AController* InstigatorController = OwnerActor->GetInstigatorController();
					TargetCharacter->TakeDamage(Damage, DamageEvent, InstigatorController, OwnerActor);

					//DrawDebugSphere(World, HitActor->GetActorLocation(), 15.0f, 12, FColor::Red, false, 1.0f);

				}
			}
		}
	}
}