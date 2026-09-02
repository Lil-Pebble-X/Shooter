// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFramework/Character.h"
#include "EnemyInterface.h"
#include "OverlayWidgetController.h"
#include "CharacterClassInfo.h"
#include "ZombieCharacter.generated.h"



class UWidgetComponent;
class UBehaviorTree;
class AZombieAIController;

UCLASS()
class DEMO_API AZombieCharacter : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieCharacter();
	
	virtual void PossessedBy(AController* NewController) override;

	/** Zombie Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** end Zombie Interface*/

	/** Combat Interface*/
	virtual void Die() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	/** end Combat Interface */ 
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;
protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "character class  Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "character class  Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Zombie;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AZombieAIController> ZombieAIController;
private:

	UPROPERTY(EditAnywhere)
	float MaxRange = 100;

};
