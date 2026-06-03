// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterUserWidget.h"

void UShooterUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

