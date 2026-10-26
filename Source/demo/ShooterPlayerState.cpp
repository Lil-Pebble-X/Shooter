// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerState.h"
#include "BaseAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "ShooterLogChannels.h"
#include "BaseAbilitySystemComponent.h"


AShooterPlayerState::AShooterPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");

	SetNetUpdateFrequency(100.f);
}

void AShooterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterPlayerState, Level);
	DOREPLIFETIME(AShooterPlayerState, Loadout);

}

UAbilitySystemComponent* AShooterPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AShooterPlayerState::SetLoadout(const FShooterLoadout& InLoadout)
{
	if (!HasAuthority()) return;

	Loadout = InLoadout;

	const FString Key = GetUniqueId().IsValid()
		? GetUniqueId()->ToString()
		: GetPlayerName();

	UE_LOG(LogShooter, Log, TEXT("[LD]SetLoadout[%s]: Primary=%s, Secondary=%s"),
		*Key,
		*GetNameSafe(Loadout.PrimaryWeapon),
		*GetNameSafe(Loadout.SecondaryWeapon));
}

void AShooterPlayerState::OnRep_Level(int32 OldLevel)
{
}

void AShooterPlayerState::OnRep_Loadout()
{
	const FString Key = GetUniqueId().IsValid()
		? GetUniqueId()->ToString()
		: GetPlayerName();

	UE_LOG(LogShooter, Log, TEXT("[LD]OnRep_Loadout[%s]: Primary=%s, Secondary=%s"),
		*Key,
		*GetNameSafe(Loadout.PrimaryWeapon),
		*GetNameSafe(Loadout.SecondaryWeapon));

}