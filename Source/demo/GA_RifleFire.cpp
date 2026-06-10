// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_RifleFire.h"

UGA_RifleFire::UGA_RifleFire()
{
    NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnly;
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}
