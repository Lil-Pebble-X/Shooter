// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAbliltySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterHUD.h"
#include "ShooterPlayerState.h"
#include "ShooterWidgetController.h"
#include "AttributeMenuWidgetController.h"

UOverlayWidgetController* UShooterAbliltySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
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

UAttributeMenuWidgetController* UShooterAbliltySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
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
