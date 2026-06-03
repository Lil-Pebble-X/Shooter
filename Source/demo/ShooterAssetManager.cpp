// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAssetManager.h"
#include "ShooterGameplayTags.h"

UShooterAssetManager& UShooterAssetManager::Get()
{	
	check(GEngine);

	UShooterAssetManager* ShooterAssetManager = Cast<UShooterAssetManager>(GEngine->AssetManager);
	return *ShooterAssetManager;
}

void UShooterAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FShooterGameplayTags::InitializeNativeGameplayTags();
}
