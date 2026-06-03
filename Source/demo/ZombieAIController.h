// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_API AZombieAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	bool IsDead() const;

protected:

	virtual void BeginPlay() override;
;
private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> AIBehavior;

};
