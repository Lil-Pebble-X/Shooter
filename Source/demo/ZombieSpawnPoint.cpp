// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSpawnPoint.h"


// Sets default values
AZombieSpawnPoint::AZombieSpawnPoint()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
}

// Called when the game starts or when spawned
void AZombieSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}


