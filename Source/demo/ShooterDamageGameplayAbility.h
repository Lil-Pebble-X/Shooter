// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterGameplayAbility.h"
#include "ShooterDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_API UShooterDamageGameplayAbility : public UShooterGameplayAbility
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
