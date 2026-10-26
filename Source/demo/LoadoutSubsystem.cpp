#include "LoadoutSubsystem.h"
#include "ShooterLogChannels.h"
#include "GameFramework/PlayerState.h"

void ULoadoutSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogShooter, Log, TEXT("LoadoutSubsystem Initialized"));
}

void ULoadoutSubsystem::Deinitialize()
{
	UE_LOG(LogShooter, Log, TEXT("LoadoutSubsystem Deinitialized"));
}

void ULoadoutSubsystem::SetLoadoutForPlayerState(APlayerState* InPlayerState, const FShooterLoadout& InLoadout)
{
	if (!IsValid(InPlayerState))
	{
		return;
	}

	const FUniqueNetIdRepl& NetId = InPlayerState->GetUniqueId();
	if (!NetId.IsValid())
	{
		return;
	}

	const FString PlayerKey = NetId->ToString();

	PlayerLoadoutsMap.Add(PlayerKey, InLoadout);

	UE_LOG(LogShooter, Log, TEXT("SetLoadout: ря╢Ф key=%s"), *PlayerKey);
}

bool ULoadoutSubsystem::GetLoadoutForPlayerState(APlayerState* InPlayerState, FShooterLoadout& OutLoadout) const
{
	if (!IsValid(InPlayerState))
	{
		return false;
	}
	const FUniqueNetIdRepl& NetId = InPlayerState->GetUniqueId();
	if (!NetId.IsValid())
	{
		return false;
	}

	const FString PlayerKey = NetId->ToString();
	const FShooterLoadout* OutLoad = PlayerLoadoutsMap.Find(PlayerKey);

	if (OutLoad)
	{
		OutLoadout = *OutLoad;
		UE_LOG(LogShooter, Log, TEXT("[LD] GetLoadout: key=%s, Primary=%s, Secondary=%s"),
			*PlayerKey,
			*GetNameSafe(OutLoad->PrimaryWeapon),
			*GetNameSafe(OutLoad->SecondaryWeapon));

		return true;
	}
	else
	{
		return false;
	}
}
