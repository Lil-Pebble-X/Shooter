// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterPlayerState.h"
#include "BaseAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "ShooterPlayerController.h"
#include "GameFramework/Controller.h"
#include "ShooterHUD.h"


// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	//Set Max Walk Speed
	WalkSpeed = 150.0f;

	//Set Max Sprint Speed
	SprintSpeed = 350.0f;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

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

FVector AShooterCharacter::GetCombatSocketLocation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void AShooterCharacter::Action_Sprint(const FInputActionValue& Value)
{
	float MaxSpeed = GetCharacterMovement()->MaxWalkSpeed;
	if (GetCharacterMovement())
	{
		if (MaxSpeed == SprintSpeed)
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		}
	}
}

void AShooterCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Init ability actor info for the  Server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AShooterCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Init ability actor info for the  Client
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
	InitializeDefaultAttributes();
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

	//Set up action binding
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AShooterCharacter::Action_Sprint);
	}
}