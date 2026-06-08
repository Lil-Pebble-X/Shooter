// Fill out your copyright notice in the Description page of Project Settings.


#include "UGA_WeaponFireBase.h"
#include "Kismet/KismetSystemLibrary.h"

void UUGA_WeaponFireBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
										  const FGameplayAbilityActorInfo* ActorInfo,
										  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	


}

void UUGA_WeaponFireBase::SpawnProjectile()
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

}

bool UUGA_WeaponFireBase::PerformHitscan(FHitResult& OutHit)
{
	const APawn* Pawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	if (!Pawn) return false;

	const FVector Start = Pawn->GetPawnViewLocation();
	const FRotator Rot = Pawn->GetControlRotation();
	const FVector End = Start + Rot.Vector() * HitscanRange;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Pawn);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		OutHit, Start, End, HitscanTraceChannel, QueryParams
	);

	DrawDebugLine(GetWorld(), Start, bHit ? OutHit.ImpactPoint : End,
		bHit ? FColor::Red : FColor::Green, false, 2.f, 0, 2.f);


	return bHit;
}
