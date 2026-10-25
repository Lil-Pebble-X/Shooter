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
		UE_LOG(LogShooter, Warning, TEXT("SetLoadout: InPlayerState 无效"));
		return;
	}

	const FUniqueNetIdRepl& NetId = InPlayerState->GetUniqueId();
	if (!NetId.IsValid())
	{
		UE_LOG(LogShooter, Warning, TEXT("SetLoadout: UniqueId 无效，无法作为 key"));
		return;
	}

	const FString PlayerKey = NetId->ToString();

	PlayerLoadoutsMap.Add(PlayerKey, InLoadout);

	UE_LOG(LogShooter, Log, TEXT("SetLoadout: 已存 key=%s"), *PlayerKey);
}

bool ULoadoutSubsystem::GetLoadoutForPlayerState(APlayerState* InPlayerState, FShooterLoadout& OutLoadout) const
{
	if (!IsValid(InPlayerState))
	{
		UE_LOG(LogShooter, Warning, TEXT("GetLoadout: InPlayerState 无效"));
		return false;
	}
	const FUniqueNetIdRepl& NetId = InPlayerState->GetUniqueId();
	if (!NetId.IsValid())
	{
		UE_LOG(LogShooter, Warning, TEXT("GetLoadout: UniqueId 无效，无法作为 key"));
		return false;
	}

	const FString PlayerKey = NetId->ToString();

	const FShooterLoadout* OutLoad = PlayerLoadoutsMap.Find(PlayerKey);

	if (OutLoad)
	{
		OutLoadout = *OutLoad;
		UE_LOG(LogShooter, Log, TEXT("GetLoadout: 已取 key=%s"), *PlayerKey);
		return true;
	}
	else
	{
		UE_LOG(LogShooter, Warning, TEXT("GetLoadout: 未找到 key=%s"), *PlayerKey);
		return false;
	}
}
