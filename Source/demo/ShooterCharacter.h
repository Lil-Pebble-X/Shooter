// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ShooterCharacter.generated.h"

class AGun;
class FInputAction;

UCLASS()
class DEMO_API AShooterCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Combat Interface */

	virtual int32 GetPlayerLevel() override;

	/** end Combat Interface */

	virtual FVector GetCombatSocketLocation() override;

	void EquipWeapon(TSubclassOf<UGameplayAbility> FireAbilityClass, USkeletalMesh* NewWeaponMesh, FName TipSocket);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitializeDefaultAttributes() const override;
	
	UFUNCTION()
	void OnRep_EquippedWeaponMesh();

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeaponMesh)
	TObjectPtr<USkeletalMesh> ReplicatedWeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Replicated, Category = "Combat")
	FName WeaponTipSocketName;

	virtual void HandleWeaponOnDeath() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<UGameplayAbility> InitialFireAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMesh> InitialWeaponMesh;
private:	
	//input system
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	TObjectPtr<UInputAction> SprintAction;

	//Input Action Function
	void Action_Sprint(const FInputActionValue& Value);

	FGameplayAbilitySpecHandle EquippedFireAbilityHandle;
	TSubclassOf<UGameplayAbility> EquippedFireAbilityClass;

	virtual void InitAbilityActorInfo() override;
};
