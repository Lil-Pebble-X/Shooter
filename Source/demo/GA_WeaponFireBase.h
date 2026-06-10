// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
 
#include "CoreMinimal.h"
#include "ShooterGameplayAbility.h"
#include "GA_WeaponFireBase.generated.h"

class AShooterProjectile;
/**
 * 
 */
UCLASS()
class DEMO_API UGA_WeaponFireBase : public UShooterGameplayAbility
{
	GENERATED_BODY()

public:

	//for RPG
	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile")
	void SpawnProjectile();

	//for most weapon
	UFUNCTION(BlueprintCallable, Category = "Weapon|Fire")
	bool PerformHitscan(FHitResult& OutHit);

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation")
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Animation")
	FGameplayTag FireNotifyTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Projectile")
	TSubclassOf<AShooterProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Hitscan")
	float HitscanRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Hitscan")
	TEnumAsByte<ECollisionChannel> HitscanTraceChannel = ECC_Visibility;

};
