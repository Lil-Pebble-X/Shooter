// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "BaseAbilitySystemcomponent.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"
#include "ShooterInputComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "EnemyInterface.h"
#include "ShooterGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"

#include"DamageTextComponent.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(ShooterContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(ShooterContext, 0);
	}

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

void AShooterPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

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

void AShooterPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount);
	}
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

void AShooterPlayerController::CursorTrace()
{
	if (GetWorld() == nullptr) return;

	FVector2D ViewportSize(0.f, 0.f);
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	else
	{
		return;
	}

	const FVector2D ScreenCenter(ViewportSize.X * 0.5f, ViewportSize.Y * 0.5f);
	FVector WorldLocation;
	FVector WorldDirection;
	if (DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldLocation, WorldDirection))
	{
		const float TraceDistance = 100000.f; 
		const FVector TraceEnd = WorldLocation + (WorldDirection * TraceDistance);

		FCollisionQueryParams QueryParams;
		if (APawn* ShooterPawn = GetPawn())
		{
			QueryParams.AddIgnoredActor(ShooterPawn);
		}

		GetWorld()->LineTraceSingleByChannel(CursorHit, WorldLocation, TraceEnd, ECollisionChannel::ECC_Visibility, QueryParams);
	}
	else
	{
		CursorHit = FHitResult();
	}

	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}
	
}

//InputTag Function
void AShooterPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FShooterGameplayTags::Get().InputTag_LMB))
	{
	}
}

void AShooterPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void AShooterPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//if (!InputTag.MatchesTagExact(FShooterGameplayTags::Get().InputTag_LMB))
	GetASC()->AbilityInputTagHeld(InputTag);
}

UBaseAbilitySystemComponent* AShooterPlayerController::GetASC()
{
	if (BaseAbilitySystemComponent == nullptr)
	{
		BaseAbilitySystemComponent =  Cast<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return BaseAbilitySystemComponent;
}

//Look
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
		
	