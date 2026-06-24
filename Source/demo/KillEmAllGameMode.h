// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterGameModeBase.h"
#include "KillEmAllGameMode.generated.h"

class UCharacterClassInfo;

/**
 * 
 */
UCLASS()
class DEMO_API AKillEmAllGameMode : public AShooterGameModeBase
{
	GENERATED_BODY()
	

public:
	virtual void PawnKilled(APawn* PawnKilled) override;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

private:
	void EndGame(bool bIsPlayerWinner);

};
