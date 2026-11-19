// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterGameModeBase.h"
#include "KillEmAllGameMode.generated.h"

class AZombieCharacter;
class AZombieSpawnPoint;

/**
 * 
 */
UCLASS()
class DEMO_API AKillEmAllGameMode : public AShooterGameModeBase
{
	GENERATED_BODY()
	

public:
	virtual void PawnKilled(APawn* PawnKilled) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<AZombieCharacter> ZombieType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning")
	int32 MaxNum = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning")
	int32 NumPerWave = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning")
	float WaveInterval = 5.0f;

	UPROPERTY()
	TArray<TObjectPtr<AZombieSpawnPoint>> ZombieSpawnPoints;

protected:
    virtual void BeginPlay() override;

	void SpawnZombies();
private:
	void EndGame(bool bIsPlayerWinner);

	FTimerHandle SpawnTimer;

	int32 SpawnedCount = 0;
};
