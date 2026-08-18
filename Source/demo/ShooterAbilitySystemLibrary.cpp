// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterHUD.h"
#include "ShooterPlayerState.h"
#include "ShooterWidgetController.h"
#include "AttributeMenuWidgetController.h"
#include "ShooterGameModeBase.h"
#include "KillEmAllGameMode.h"
#include "AbilitySystemComponent.h"
#include "ShooterAbilityTypes.h"

UOverlayWidgetController* UShooterAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AShooterHUD* ShooterHUD = Cast<AShooterHUD>(PC->GetHUD()))
		{
			AShooterPlayerState* PS = PC->GetPlayerState<AShooterPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return ShooterHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UShooterAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AShooterHUD* ShooterHUD = Cast<AShooterHUD>(PC->GetHUD()))
		{
			AShooterPlayerState* PS = PC->GetPlayerState<AShooterPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return ShooterHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

void UShooterAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());

	if (CharacterClassInfo->ShieldRegenEffect)
	{
		FGameplayEffectContextHandle RegenContext = ASC->MakeEffectContext();
		RegenContext.AddSourceObject(AvatarActor);
		const FGameplayEffectSpecHandle RegenSpec = ASC->MakeOutgoingSpec(
			CharacterClassInfo->ShieldRegenEffect, Level, RegenContext);
		ASC->ApplyGameplayEffectSpecToSelf(*RegenSpec.Data.Get());
	}
}

void UShooterAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}

}

UCharacterClassInfo* UShooterAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AShooterGameModeBase* ShooterGameMode = Cast<AShooterGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (ShooterGameMode == nullptr) return nullptr;

	return ShooterGameMode->CharacterClassInfo;
}

bool UShooterAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FShooterGameplayEffectContext* ShooterEffectContext = static_cast<const FShooterGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ShooterEffectContext->IsCriticalHit();
	}
	return false;
}

void UShooterAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FShooterGameplayEffectContext* ShooterEffectContext = static_cast<FShooterGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ShooterEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}
