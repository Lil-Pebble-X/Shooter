// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderCrosshair.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrosshairTargetDataSignature, const FVector&, Data);

/**
 * 
 */
UCLASS()
class DEMO_API UTargetDataUnderCrosshair : public UAbilityTask
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisPlayName = "TargetDataUnderCrosshair", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UTargetDataUnderCrosshair* CreateTargetDataUnderCrosshair(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FCrosshairTargetDataSignature ValidData;

private:

	virtual void Activate() override;	
};
