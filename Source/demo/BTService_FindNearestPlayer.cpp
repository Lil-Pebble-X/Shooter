// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindNearestPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	APawn* OwningPawn = AIOwner->GetPawn();

	const FName  TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Zombie") : FName("Player");
	
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag);

	for (AActor* Actor : ActorsWithTag)
	{
		GEngine->AddOnScreenDebugMessage(2, .5f, FColor::Orange, *Actor->GetName());
	}
}
