// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA_WeaponFireBase.h"
#include "GA_LauncherFire.generated.h"

/**
 * 
 */
UCLASS()
class DEMO_API UGA_LauncherFire : public UGA_WeaponFireBase
{
	GENERATED_BODY()

public:
	UGA_LauncherFire();


	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Projectile")
	TSubclassOf<AShooterProjectile> ProjectileClass;
};
