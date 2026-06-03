// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AttackNotifyState.generated.h"

class ABaseCharacter;

/**
 * 
 */
UCLASS()
class DEMO_API UAttackNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	UAttackNotifyState();

	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation, 
		float TotalDuration,
		const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation, 
		const FAnimNotifyEventReference& EventReference) override;

protected:


	void PerformDamageCheck(USkeletalMeshComponent* MeshComp);

	UPROPERTY(EditAnywhere, Category = "Damage")
	float DetectRadius;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float DamageAmount;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float AngleLimit;

	UPROPERTY(EditAnywhere, Category = "Combat|Filter")
	TSubclassOf<AActor> IgnoreActorClass;
};
