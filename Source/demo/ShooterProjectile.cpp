// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/StaticMesh.h"
#include "demo.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ShooterGameplayTags.h"
#include "TimerManager.h"
#include "AbilitySystemComponent.h"

AShooterProjectile::AShooterProjectile()
{	
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(Sphere);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

}

void AShooterProjectile::BeginPlay()
{
	Super::BeginPlay();
	//SetLifeSpan(LifeSpan);//Dangerous, may cause crashes; needs to be fixed in the future.
	SetReplicateMovement(true);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AShooterProjectile::OnSphereOverlap);
	
	//Security Risks
	//LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void AShooterProjectile::Destroyed()
{
	Super::Destroyed();
}

void AShooterProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (DamageEffectSpecHandle.Data.IsValid() && DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor)
	{
		return;
	}

	if (bHit) return;
	bHit = true;

	if (!HasAuthority()) return;

	ScheduleImpactCue();

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());

		if (StatusEffectSpecHandle.IsValid())
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*StatusEffectSpecHandle.Data.Get());
		}
	}

	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (ProjectileMovement) { ProjectileMovement->StopMovementImmediately(); }
	SetLifeSpan(0.15f);
}

void AShooterProjectile::ScheduleImpactCue()
{
	UAbilitySystemComponent* SourceASC = nullptr;
	if (DamageEffectSpecHandle.Data.IsValid())
	{
		SourceASC = DamageEffectSpecHandle.Data->GetContext().GetInstigatorAbilitySystemComponent();
	}
	if (!SourceASC)
	{
		SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	}
	if (!SourceASC) return;

	const FVector CueLocation = GetActorLocation();
	const FVector CueNormal = GetActorForwardVector();
	AActor* Avatar = SourceASC->GetAvatarActor();
	TWeakObjectPtr<UAbilitySystemComponent> WeakASC = SourceASC;

	GetWorldTimerManager().SetTimerForNextTick([WeakASC, CueLocation, CueNormal, Avatar]()
		{
			UAbilitySystemComponent* ASC = WeakASC.Get();
			if (!ASC) return;

			FGameplayCueParameters CueParams;
			CueParams.Location = CueLocation;
			CueParams.Normal = CueNormal;
			CueParams.Instigator = Avatar;
			CueParams.EffectCauser = Avatar;  

			ASC->ExecuteGameplayCue(FShooterGameplayTags::Get().GameplayCue_Weapon_Impact, CueParams);
		});
}