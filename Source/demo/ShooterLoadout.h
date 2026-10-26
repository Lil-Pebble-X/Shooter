#pragma once

#include "CoreMinimal.h"
#include "ShooterWeaponData.h"
#include "ShooterLoadout.generated.h"

USTRUCT(BlueprintType)
struct FShooterLoadout
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	TObjectPtr<UShooterWeaponData> PrimaryWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	TObjectPtr<UShooterWeaponData> SecondaryWeapon;
};