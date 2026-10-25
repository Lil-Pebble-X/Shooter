#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ShooterLoadout.h"
#include "GameFramework/PlayerState.h"
#include "LoadoutSubsystem.generated.h"

UCLASS()
class DEMO_API ULoadoutSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "LoadOut")
    void SetLoadoutForPlayerState(APlayerState* InPlayerState, const FShooterLoadout& InLoadout);

    UFUNCTION(BlueprintCallable, Category = "LoadOut")
    bool GetLoadoutForPlayerState(APlayerState* InPlayerState, FShooterLoadout& OutLoadout) const;

private:

	UPROPERTY()
	TMap<FString, FShooterLoadout> PlayerLoadoutsMap;
};