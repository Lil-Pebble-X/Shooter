// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieSpawnPoint.h"
#include "Components/BillboardComponent.h"

// Sets default values
AZombieSpawnPoint::AZombieSpawnPoint()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Billboard = CreateDefaultSubobject<UBillboardComponent>("Billboard");
	Billboard->SetupAttachment(Root);
	Billboard->bIsScreenSizeScaled = true;
	Billboard->SetRelativeScale3D(FVector(0.5f));
}

// Called when the game starts or when spawned
void AZombieSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}


