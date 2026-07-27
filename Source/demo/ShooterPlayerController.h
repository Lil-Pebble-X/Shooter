// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "ShooterPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UShooterInputConfig;
class UBaseAbilitySystemComponent;
class IEnemyInterface;
class UDamageTextComponent;

/**
 * 
 */
UCLASS()
class DEMO_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AShooterPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bCriticalHit);
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget>	WinScreenClass;

	UPROPERTY(EditAnywhere)
	float RestartDelay = 5;

	FTimerHandle RestartTimer;	

	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	TObjectPtr<UInputMappingContext> ShooterContext;

	//input system
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	TObjectPtr<UInputAction> JumpAction;

	//Input Action Function
	void Action_Look(const FInputActionValue& Value);

	void Action_Jump(const FInputActionValue& Value);

	void Action_Move(const FInputActionValue& Value);

	void CursorTrace();
	FHitResult CursorHit;
	IEnemyInterface* ThisActor;
	IEnemyInterface* LastActor;

	//InputTag Function
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInput")
	TObjectPtr<UShooterInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UBaseAbilitySystemComponent> BaseAbilitySystemComponent;

	UBaseAbilitySystemComponent* GetASC();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
