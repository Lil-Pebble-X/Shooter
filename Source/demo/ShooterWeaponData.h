// ShooterWeaponData.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ShooterWeaponData.generated.h"

class UGameplayAbility;
class USkeletalMesh;

UCLASS(BlueprintType)
class DEMO_API UShooterWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** 开火技能 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UGameplayAbility> FireAbilityClass;

	/** 武器模型 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USkeletalMesh> WeaponMesh;

	/** 枪口插槽名 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TipSocketName;

	/** 装备页显示名 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FText DisplayName;
};
