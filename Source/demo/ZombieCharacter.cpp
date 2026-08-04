// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseAbilitySystemComponent.h"
#include "BaseAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "ShooterUserWidget.h"
#include "ShooterAbilitySystemLibrary.h"
#include "demo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "ZombieAIController.h"
#include "ShooterGameplayTags.h"

// Sets default values
AZombieCharacter::AZombieCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	IsAttack = false;

	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AZombieCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ZombieAIController = Cast<AZombieAIController>(NewController);
}

void AZombieCharacter::Scratch()
{
	IsAttack = true;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			IsAttack = false;
		}, 1.0f, false);

}

void AZombieCharacter::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);	
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

}

void AZombieCharacter::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);	
}

 bool AZombieCharacter::FinishAttack() const
{
	return  IsAttack;
}

// Called when the game starts or when spawned
void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	if (HasAuthority())
	{
		InitAbilityActorInfo();
		UShooterAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);
	}

	if (UShooterUserWidget* ShooterUserWidget = Cast<UShooterUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		ShooterUserWidget->SetWidgetController(this);
	}

	if(const UBaseAttributeSet* ShooterAS = Cast<UBaseAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ShooterAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data) 
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ShooterAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		OnHealthChanged.Broadcast(ShooterAS->GetHealth());
		OnMaxHealthChanged.Broadcast(ShooterAS->GetMaxHealth());
	}
	
}

void AZombieCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

void AZombieCharacter::InitializeDefaultAttributes() const
{
	UShooterAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AZombieCharacter::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}

// Called every frame
void AZombieCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZombieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

