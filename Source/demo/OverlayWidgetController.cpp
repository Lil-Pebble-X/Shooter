// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidgetController.h"
#include "BaseAbilitySystemComponent.h"
#include "BaseAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UBaseAttributeSet* ShooterAttributeSet = CastChecked<UBaseAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(ShooterAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(ShooterAttributeSet->GetMaxHealth());
	OnShieldChanged.Broadcast(ShooterAttributeSet->GetShield());
	OnMaxShieldChanged.Broadcast(ShooterAttributeSet->GetMaxShield());
	OnAttackPowerChanged.Broadcast(ShooterAttributeSet->GetAttackPower());

}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UBaseAttributeSet* ShooterAttributeSet = CastChecked<UBaseAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ShooterAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data) 
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ShooterAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ShooterAttributeSet->GetShieldAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnShieldChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ShooterAttributeSet->GetMaxShieldAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxShieldChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ShooterAttributeSet->GetAttackPowerAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnAttackPowerChanged.Broadcast(Data.NewValue);
			}
		);

	Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				//A.1".MatchesTag("A") will return True, "A".MatchesTag("A.1") will return False
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message")); 
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);
}
