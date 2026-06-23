// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "ShooterGameplayTags.h"
#include "GameplayTagsManager.h"



UBaseAttributeSet::UBaseAttributeSet()
{
	const FShooterGameplayTags& GameplayTags = FShooterGameplayTags::Get();

	TagsToAttributes.Add(GameplayTags.Attributes_Primary_ElementalMastery, GetElementalMasteryAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_CritRate, GetCritRateAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_AttackPower, GetAttackPowerAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_CritDmg, GetCritDmgAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_MaxCritRate, GetMaxCritRateAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Speed, GetSpeedAttribute);
	TagsToAttributes.Add(UGameplayTagsManager::Get().RequestGameplayTag(FName("Attributes.Vital.MaxHealth")),
		GetMaxHealthAttribute);
	TagsToAttributes.Add(UGameplayTagsManager::Get().RequestGameplayTag(FName("Attributes.Vital.MaxShield")),
		GetMaxShieldAttribute);
}

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	//
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, ElementalMastery, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CritRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxCritRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CritDmg, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Speed, COND_None, REPNOTIFY_Always);
	
	//
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxShield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxShield());
	}

	if (Attribute == GetCritRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxCritRate());
	}
}

void UBaseAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxShield());
	}

	if (Attribute == GetCritRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxCritRate());
	}
}

void UBaseAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		UE_LOG(LogTemp, Warning, TEXT("Changed Health on %s, Health: %f"), *Props.TargetAvatarActor->GetName(), GetHealth());
	}
	if (Data.EvaluatedData.Attribute == GetShieldAttribute())
	{
		SetShield(FMath::Clamp(GetShield(), 0.f, GetMaxShield()));
	}
}

//
void UBaseAttributeSet::OnRep_ElementalMastery(const FGameplayAttributeData& OldElementalMastery) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, ElementalMastery, OldElementalMastery);
}

void UBaseAttributeSet::OnRep_CritRate(const FGameplayAttributeData& OldCritRate) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CritRate, OldCritRate);
}

void UBaseAttributeSet::OnRep_MaxCritRate(const FGameplayAttributeData& OldMaxCritRate) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxCritRate, OldMaxCritRate);
}

void UBaseAttributeSet::OnRep_CritDmg(const FGameplayAttributeData& OldCritDmg) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CritDmg, OldCritDmg);
}

void UBaseAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldShield) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Shield, OldShield);
}

void UBaseAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, AttackPower, OldAttackPower);
}

void UBaseAttributeSet::OnRep_Speed(const FGameplayAttributeData& OldSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Speed, OldSpeed);
}

//

void UBaseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health, OldHealth);
}


void UBaseAttributeSet::OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxShield, OldMaxShield);
}

void UBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldMaxHealth);
}