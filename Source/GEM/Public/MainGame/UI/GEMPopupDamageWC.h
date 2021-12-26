// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameplayTagContainer.h"
#include "GEMPopupDamageWC.generated.h"

/**
 * 
 */
UCLASS()
class GEM_API UGEMPopupDamageWC : public UWidgetComponent
{
	GENERATED_BODY()

public:
    // Called to setup functionality for our widget
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Damage Popup")
	void SetupDamagePopup(float InDamage, FGameplayTagContainer InTags);
};
