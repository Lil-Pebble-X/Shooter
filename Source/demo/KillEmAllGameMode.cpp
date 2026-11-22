// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ZombieSpawnPoint.h"
#include "TimerManager.h"
#include "ZombieCharacter.h"
#include "ShooterLogChannels.h"
#include "ZombieAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if (PlayerController != nullptr)
	{
		EndGame(false);
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
	UE_LOG(LogShooter, Log, TEXT("SpawnZombies tick"));
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
	AZombieSpawnPoint* ZombieSpawnPoint = ZombieSpawnPoints[0];
	int32 NumToSpawn = FMath::Min(NumPerWave, MaxNum - SpawnedCount);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	int32 SpawnedThisWave = 0;
	for (int32 i = 0; i < NumToSpawn; i++)
	{
		AZombieCharacter* Zombie = GetWorld()->SpawnActor<AZombieCharacter>(ZombieType, ZombieSpawnPoint->GetActorLocation(), ZombieSpawnPoint->GetActorRotation(), SpawnParams);
		if (Zombie)
		{
			SpawnedCount++;
			SpawnedThisWave++;
		}
	}
	UE_LOG(LogShooter, Log, TEXT("Wave: requested %d, spawned %d, total %d/%d"), NumToSpawn, SpawnedThisWave, SpawnedCount, MaxNum);
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}
