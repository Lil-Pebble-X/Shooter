#pragma once

#include "CoreMinimal.h"
#include "ShooterWeaponData.h"
#include "ShooterLoadout.generated.h"

/** 装备槽位，用于菜单选择与装备映射 */
UENUM(BlueprintType)
enum class EWeaponSlot :uint8
{
	Primary UMETA(DisplayName = "主武器"),
	Secondary UMETA(DisplayName = "副武器")
};

USTRUCT(BlueprintType)
struct FShooterLoadout
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	TObjectPtr<UShooterWeaponData> PrimaryWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loadout")
	TObjectPtr<UShooterWeaponData> SecondaryWeapon;
};