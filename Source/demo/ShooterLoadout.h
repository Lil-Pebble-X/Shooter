#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "ShooterLoadout.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FShooterLoadout
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	TSubclassOf<UGameplayAbility> PrimaryWeaponAbility;
};
