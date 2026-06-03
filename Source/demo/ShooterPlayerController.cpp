// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"
#include "ShooterInputComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(ShooterContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(ShooterContext, 0);

	//bShowMouseCursor = true;
	//DefaultMouseCursor = EMouseCursor::Default;

	//FInputModeGameAndUI InputModeData;
	//InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//InputModeData.SetHideCursorDuringCapture(false);
	//SetInputMode(InputModeData);
}

void AShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UShooterInputComponent* ShooterInputComponent = CastChecked<UShooterInputComponent>(InputComponent);

	//Move
	ShooterInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShooterPlayerController::Action_Move);

	//Look
	ShooterInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShooterPlayerController::Action_Look);

	//Jump
	ShooterInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AShooterPlayerController::Action_Jump);

	ShooterInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

AShooterPlayerController::AShooterPlayerController()
{
	bReplicates = true;
}

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (bIsWinner)
	{
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
		if (WinScreen != nullptr)
		{
			WinScreen->AddToViewport();
		}
	}
	else
	{
		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
		if (LoseScreen != nullptr)
		{
			LoseScreen->AddToViewport();
		}
	}

	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

void AShooterPlayerController::Action_Move(const FInputActionValue& Value)
{
	const FVector2D MoveAxisVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();

	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, MoveAxisVector.Y);

		ControlledPawn->AddMovementInput(RightDirection, MoveAxisVector.X);
	}
}

void AShooterPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void AShooterPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Green, *InputTag.ToString());
}

void AShooterPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Blue, *InputTag.ToString());
}

void AShooterPlayerController::Action_Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	FRotator Rotation = GetControlRotation();

	const float Yaw = Rotation.Yaw + LookAxisVector.X;

	float Pitch = Rotation.Pitch + LookAxisVector.Y;

	Pitch = FMath::Clamp(Pitch, -80.0f, 90.0f);

	if (GetPawn<APawn>())
	{
		SetControlRotation(FRotator(Pitch, Yaw, 0.f));
	}
}

void AShooterPlayerController::Action_Jump(const FInputActionValue& Value)
{
	ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());
	if (ControlledCharacter)
	{
		ControlledCharacter->Jump();
	}
}
		
	