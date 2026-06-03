// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterWidgetController.h"

void UShooterWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCparams)
{
	PlayerController = WCparams.PlayerController;
	PlayerState = WCparams.PlayerState;
	AbilitySystemComponent = WCparams.AbilitySystemComponent;
	AttributeSet = WCparams.AttributeSet;
}

void UShooterWidgetController::BroadcastInitialValues()
{
}

void UShooterWidgetController::BindCallbacksToDependencies()
{

}
