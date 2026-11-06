#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ShooterLoadout.h"
#include "ShooterWeaponCatalog.generated.h"

class UShooterWeaponData;

UCLASS(BlueprintType)
class DEMO_API UShooterWeaponCatalog : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * 可选武器列表。
	 * 索引即武器 ID —— ServerSetLoadout 的 RPC 参数就是这个索引。
	 * 只许在末尾追加，不许重排、不许删除中间项，否则旧存档与 RPC 会错位。
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TArray<TObjectPtr<UShooterWeaponData>> Weapons;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Weapon")
	bool TryMakeLoadout(int32 PrimaryIndex, int32 SecondaryIndex, FShooterLoadout& OutLoadout) const;
};