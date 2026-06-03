// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Scratch.h"
#include "AIController.h"
#include "ZombieCharacter.h"

UBTTask_Scratch::UBTTask_Scratch()
{
	NodeName = "Scratch";
}

EBTNodeResult::Type UBTTask_Scratch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AZombieCharacter* Character = Cast<AZombieCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Character->Scratch();

	return EBTNodeResult::Succeeded;
}
