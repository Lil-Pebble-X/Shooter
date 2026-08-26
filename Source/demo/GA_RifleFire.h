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

protected:

	virtual bool ValidateTarget(const FHitResult& HitResult) const override;
	virtual void OnLocalFireEffects(const FHitResult& HitResult) override;
	virtual void OnServerFireResolved(const FHitResult& HitResult) override;

	void ApplyDamageToTarget(AActor* TargetActor, const FHitResult& HitResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Effects")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Effects")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Effects")
	TObjectPtr<UNiagaraSystem> TracerEffect;
};
