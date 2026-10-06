// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_FindNearestPlayer.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_API UBTService_FindNearestPlayer : public UBTService_BlueprintBase
{
	GENERATED_BODY()

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector TargetToFollowSelector;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector DistanceToTargetSelector;

	//Perception Parameters 
	UPROPERTY(EditAnywhere, Category = "Perception")
	float SightRadius = 1500.f;

	UPROPERTY(EditAnywhere, Category = "Perception")
	bool bRequireLineOfSight = true;

	UPROPERTY(EditAnywhere, Category = "Perception")
	float EyeHeightOffset = 60.f;

	UPROPERTY(EditAnywhere, Category = "Perception")
	TEnumAsByte<ECollisionChannel> SightTraceChannel = ECC_Visibility;

	bool CanSeeTarget(const APawn* OwningPawn, const AActor* Target) const;
};
