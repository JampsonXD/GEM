// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGame/UI/UW_Crosshair.h"

void UUW_Crosshair::ToggleCrosshair(bool bToggleOn)
{
	const ESlateVisibility CrosshairVisibility = bToggleOn ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed;
	Crosshair_Overlay->SetVisibility(CrosshairVisibility);
}

void UUW_Crosshair::SetWeaponCrosshairTexture(UTexture2D* InCrosshairTexture)
{
	Crosshair_Image->SetBrushFromTexture(InCrosshairTexture);
}
