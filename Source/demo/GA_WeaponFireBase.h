// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
 
#include "CoreMinimal.h"
#include "ShooterDamageGameplayAbility.h"
#include "GA_WeaponFireBase.generated.h"

class AShooterProjectile;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class DEMO_API UGA_WeaponFireBase : public UShooterDamageGameplayAbility
{
	GENERATED_BODY()

public:

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;	

	void StartTargeting();                      
	UFUNCTION()
	void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& DataHandle);

	virtual void OnLocalFireEffects(const FHitResult& HitResult);    
	virtual void OnServerFireResolved(const FHitResult& HitResult); 
	virtual bool ValidateTarget(const FHitResult& HitResult) const;
	
	FGameplayEffectSpecHandle BuildDamageSpec(const FHitResult& HitResult); 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation")
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation")
	FGameplayTag FireNotifyTag;

};
