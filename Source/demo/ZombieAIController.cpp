// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ZombieCharacter.h"

void AZombieAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);

		APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}

void AZombieAIController::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);
}

bool AZombieAIController::IsDead() const
{
	AZombieCharacter* ControlledCharacter = Cast<AZombieCharacter>(GetPawn());
	if (ControlledCharacter != nullptr)
	{
		return ControlledCharacter->IsDead();
	}

	return true;
}