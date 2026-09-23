// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterAbilitySystemLibrary.h"
#include "ShooterPlayerState.h"
#include "BaseAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "ShooterPlayerController.h"
#include "GameFramework/Controller.h"
#include "ShooterHUD.h"
#include "Net/UnrealNetwork.h"
#include "ShooterDamageGameplayAbility.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Weapon->SetVisibility(true);
	Weapon->SetupAttachment(GetMesh(), FName("WeaponSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// if change weapon
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
}

void AShooterCharacter::InitializeDefaultAttributes() const
{
	UShooterAbilitySystemLibrary::InitializeDefaultAttributes(this, ECharacterClass::Soldier, 1.f, AbilitySystemComponent);
}

void AShooterCharacter::OnRep_EquippedWeaponMesh()
{
	if (Weapon && ReplicatedWeaponMesh)
	{
		Weapon->SetSkeletalMesh(ReplicatedWeaponMesh);
	}
}

void AShooterCharacter::HandleWeaponOnDeath()
{
	if (!Weapon) return;

	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

}

FVector AShooterCharacter::GetCombatSocketLocation_Implementation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void AShooterCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the  Server
	InitAbilityActorInfo();
	AddCharacterAbilities();
	InitializeDefaultAttributes();
	EquipWeapon(InitialFireAbility, InitialWeaponMesh, WeaponTipSocketName);
}

void AShooterCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the  Client
	InitAbilityActorInfo();
}


int32 AShooterCharacter::GetPlayerLevel()
{
	const AShooterPlayerState* ShooterPlayerState = GetPlayerState<AShooterPlayerState>();
	check(ShooterPlayerState);
	return ShooterPlayerState->GetPlayerLevel();
}

void AShooterCharacter::InitAbilityActorInfo()
{
	AShooterPlayerState* ShooterPlayerState = GetPlayerState<AShooterPlayerState>();
	check(ShooterPlayerState);
	ShooterPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(ShooterPlayerState, this);
	Cast<UBaseAbilitySystemComponent>(ShooterPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = ShooterPlayerState->GetAbilitySystemComponent();
	AttributeSet = ShooterPlayerState->GetAttributeSet();

	if (AShooterPlayerController* ShooterPlayerController = Cast<AShooterPlayerController>(GetController()))
	{
		if (AShooterHUD* ShooterHUD = Cast<AShooterHUD>(ShooterPlayerController->GetHUD()))
		{
			ShooterHUD->InitOverlay(ShooterPlayerController, ShooterPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	BindAttributeDelegates();
}


// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void AShooterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AShooterCharacter, WeaponTipSocketName);
	DOREPLIFETIME(AShooterCharacter, ReplicatedWeaponMesh);
}

void AShooterCharacter::EquipWeapon(TSubclassOf<UGameplayAbility> FireAbilityClass, USkeletalMesh* NewWeaponMesh, FName TipSocket)
{


	if (!AbilitySystemComponent || !FireAbilityClass) return;

	if (EquippedFireAbilityClass == FireAbilityClass) return;

	// Delete Old Weapon Fire Ability
	if (EquippedFireAbilityHandle.IsValid())
	{
		AbilitySystemComponent->CancelAbilityHandle(EquippedFireAbilityHandle);

		AbilitySystemComponent->ClearAbility(EquippedFireAbilityHandle);

		EquippedFireAbilityHandle = FGameplayAbilitySpecHandle();
	}

	// Grant New Fire Ability
	FGameplayAbilitySpec Spec(FireAbilityClass, 1);
	if (const UShooterGameplayAbility* ShooterAbility = Cast<UShooterGameplayAbility>(Spec.Ability))
	{
		Spec.GetDynamicSpecSourceTags().AddTag(ShooterAbility->StartupInputTag);
	}
	EquippedFireAbilityHandle = AbilitySystemComponent->GiveAbility(Spec);
	EquippedFireAbilityClass = FireAbilityClass;


	// Swap Weapon Mesh 
	if (Weapon && NewWeaponMesh)
	{
		Weapon->SetSkeletalMesh(NewWeaponMesh);
		WeaponTipSocketName = TipSocket;
	}

	if (NewWeaponMesh)
	{
		ReplicatedWeaponMesh = NewWeaponMesh;   // Replicate to Client
		if (Weapon)
		{
			Weapon->SetSkeletalMesh(NewWeaponMesh);  
		}
		WeaponTipSocketName = TipSocket;
	}
}