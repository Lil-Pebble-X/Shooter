// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindNearestPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "CombatInterface.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	APawn* OwningPawn = AIOwner->GetPawn();

	if (!IsValid(OwningPawn))
	{
		UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, nullptr);
		UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, TNumericLimits<float>::Max());
		return;
	}

	const FName  TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Zombie") : FName("Player");
	
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag);

	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;
	for (AActor* Actor : ActorsWithTag)
	{

		if (IsValid(Actor) && IsValid(OwningPawn))
		{
			if (Actor->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Actor)) continue;

			const float Distance = OwningPawn->GetDistanceTo(Actor);

			if (Distance > SightRadius || Distance >= ClosestDistance) continue;

			if (!CanSeeTarget(OwningPawn, Actor)) continue;

			ClosestDistance = Distance;
			ClosestActor = Actor;
		}
	}
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);
}

bool UBTService_FindNearestPlayer::CanSeeTarget(const APawn* OwningPawn, const AActor* Target) const
{
	if (!OwningPawn || !Target) return false;
	if (!bRequireLineOfSight) return true;
	if (!GetWorld()) return false;

	const FVector EyeLocation = OwningPawn->GetActorLocation() + FVector(0.f, 0.f, EyeHeightOffset);

	FVector TargetLocation = Target->GetActorLocation();
	if (const ACharacter* TargetCharacter = Cast<ACharacter>(Target))
	{
		if (const UCapsuleComponent* Capsule = TargetCharacter->GetCapsuleComponent())
		{
			TargetLocation = Capsule->GetComponentLocation();
		}
	}

	FCollisionQueryParams QueryParams(TEXT("ZombieSightTrace"), /*bTraceComplex=*/false, OwningPawn);

	FHitResult Hit;
	const bool bHitSomething = GetWorld()->LineTraceSingleByChannel(
		Hit, EyeLocation, TargetLocation, SightTraceChannel, QueryParams);

	const bool bVisible = !bHitSomething || Hit.GetActor() == Target;

	return bVisible;
}
