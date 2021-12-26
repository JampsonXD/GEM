// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "UW_Crosshair.generated.h"

/**
 * 
 */
UCLASS()
class GEM_API UUW_Crosshair : public UUserWidget
{
	GENERATED_BODY()

public:

	// Toggles the Overlay on or off
	UFUNCTION()
	void ToggleCrosshair(bool bToggleOn);

	UFUNCTION()
	void SetWeaponCrosshairTexture(UTexture2D* InCrosshairTexture);
	
protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* Crosshair_Overlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* Crosshair_Image;
	
};
