// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA_WeaponFireBase.h"
#include "GA_RifleFire.generated.h"


class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class DEMO_API UGA_RifleFire : public UGA_WeaponFireBase
{
	GENERATED_BODY()
	
public:
	UGA_RifleFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Fire")
	void FireHitscan();



protected:

	void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& DataHandle);
	void ApplyDamageToTarget(AActor* TargetActor, const FHitResult& HitResult);
	void PlayFireEffects(const FHitResult& HitResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Hitscan")
	float HitscanRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Hitscan")
	TEnumAsByte<ECollisionChannel> HitscanTraceChannel = ECC_Visibility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Effects")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Effects")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Effects")
	TObjectPtr<UNiagaraSystem> TracerEffect;
};
