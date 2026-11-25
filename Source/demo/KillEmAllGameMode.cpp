// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ZombieSpawnPoint.h"
#include "TimerManager.h"
#include "ZombieCharacter.h"
#include "ShooterLogChannels.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	// T2.1: player death -> EndGame disabled, restart rules to be decided separately
	/*
	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if (PlayerController != nullptr)
	{
		EndGame(false);
	}
	*/

	AZombieCharacter* ZombieCharacter = Cast<AZombieCharacter>(PawnKilled);
	if (ZombieCharacter != nullptr)
	{
		AliveCount--;
		UE_LOG(LogShooter, Log, TEXT("Zombie Died: alive %d/%d"), AliveCount, MaxAlive);
	}
}

void AKillEmAllGameMode::BeginPlay()
{
	Super::BeginPlay();

	for (AZombieSpawnPoint* ZombieSpawnPoint : TActorRange<AZombieSpawnPoint>(GetWorld()))
	{
		ZombieSpawnPoints.Add(ZombieSpawnPoint);
	}
	UE_LOG(LogShooter, Log, TEXT("Point:%d"), ZombieSpawnPoints.Num());
	if (ZombieSpawnPoints.Num() == 0) 
	{ 
		return;
	}
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AKillEmAllGameMode::SpawnZombies, WaveInterval, true);
}

void AKillEmAllGameMode::SpawnZombies()
{
	if (!ZombieType)
	{
		UE_LOG(LogShooter, Warning, TEXT("No ZombieType"));
		return;
	}
	if (SpawnedCount >= MaxNum)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimer);
		return;
	}

	TArray<AZombieSpawnPoint*> ActivePoints;
	for (AZombieSpawnPoint* Point : ZombieSpawnPoints)
	{
		if (Point && Point->IsActive())
		{
			ActivePoints.Add(Point);
		}
	}
	if (ActivePoints.Num() <= 0)
	{
		UE_LOG(LogShooter, Warning, TEXT("No Active Points"));
		return;
	}

	int32 NumToSpawn = FMath::Min3(NumPerWave, MaxNum - SpawnedCount, MaxAlive - AliveCount);
	if (NumToSpawn <= 0)
	{
		UE_LOG(LogShooter, Log, TEXT("Alive cap reached"));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	int32 SpawnedThisWave = 0;

	for (int32 i = 0; i < NumToSpawn; i++)
	{
		AZombieSpawnPoint* Point = ActivePoints[(NextPointIndex + i) % ActivePoints.Num()];
		AZombieCharacter* Zombie = GetWorld()->SpawnActor<AZombieCharacter>(ZombieType, Point->GetActorLocation(), Point->GetActorRotation(), SpawnParams);
		if (Zombie)
		{
			SpawnedCount++;
			AliveCount++;
			SpawnedThisWave++;
		}
	}
	NextPointIndex = (NextPointIndex + NumToSpawn) % ActivePoints.Num();

	UE_LOG(LogShooter, Log, TEXT("Wave: requested %d, spawned %d, total %d/%d, alive %d/%d"), NumToSpawn, SpawnedThisWave, SpawnedCount, MaxNum, AliveCount, MaxAlive);

	if (SpawnedThisWave == 0)
	{
		ConsecutiveFailedWaves++;
		UE_LOG(LogShooter, Log, TEXT("Consecutive failed waves: %d/%d"), ConsecutiveFailedWaves, MaxFailedWaves);
		if (ConsecutiveFailedWaves >= MaxFailedWaves)
		{
			UE_LOG(LogShooter, Warning, TEXT("spawning stopped, %d consecutive failed waves, spawned %d/%d "), ConsecutiveFailedWaves, SpawnedCount, MaxNum);
			GetWorldTimerManager().ClearTimer(SpawnTimer);
		}
	}
	else
	{
		ConsecutiveFailedWaves = 0;
	}
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}
