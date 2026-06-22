// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFramework/Character.h"
#include "EnemyInterface.h"
#include "OverlayWidgetController.h"
#include "ZombieCharacter.generated.h"


class UWidgetComponent;

UCLASS()
class DEMO_API AZombieCharacter : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieCharacter();

	void Scratch();

	/** Zombie Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** end Zombie Interface*/

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	// Attack
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool IsAttack;

	UFUNCTION(BlueprintPure)
	bool FinishAttack() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

private:

	UPROPERTY(EditAnywhere)
	float MaxRange = 100;

};
