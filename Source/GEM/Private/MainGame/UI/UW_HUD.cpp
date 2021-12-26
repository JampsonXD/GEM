// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGame/UI/UW_HUD.h"

#include "MainGame/UI/UW_ExperiencePopup.h"

void UUW_HUD::SetWeaponDataInfo_Implementation(UGEMWeaponData* InWeaponData)
{
	WeaponInfo->SetWeaponInfoFromWeaponData(InWeaponData);
	SetWeaponCrosshair(InWeaponData->WeaponCrosshair);
}

void UUW_HUD::SetWeaponName(FName InWeaponName)
{
	WeaponInfo->SetWeaponName(InWeaponName);
}

void UUW_HUD::SetWeaponCurrentAmmoText(int InCurrentAmmoCount)
{
	WeaponInfo->SetWeaponCurrentAmmo(InCurrentAmmoCount);
}

void UUW_HUD::SetWeaponMaximumAmmoText(int InMaximumAmmoCount)
{
	WeaponInfo->SetWeaponMaximumAmmo(InMaximumAmmoCount);
}

void UUW_HUD::SetWeaponReserveAmmoText(int InReserveAmmoCount)
{
	WeaponInfo->SetWeaponReserveAmmoText(InReserveAmmoCount);
}

void UUW_HUD::ToggleWeaponInfoVisibility(ESlateVisibility InVisibility)
{
	WeaponInfo->SetVisibility(InVisibility);
}

void UUW_HUD::PlayDeathRecap(UGEMCharacterData* InEnemyCharacterData)
{
	PlayerDeathRecap->PlayDeathRecap(InEnemyCharacterData);
}

void UUW_HUD::AddExperiencePopup(float InExperienceGained)
{
	// Create an Experience Popup Widget
	UUW_ExperiencePopup* ExperiencePopup = Cast<UUW_ExperiencePopup>(CreateWidget(this, ExperiencePopupClass));
	ExperienceOverlay->AddChildToOverlay(ExperiencePopup);
	ExperiencePopup->SetExperienceAmount(InExperienceGained);
	ExperiencePopup->ShowExperience();
}

void UUW_HUD::SetupInitialHealth(UAbilitySystemComponent* ASC)
{
	PlayerStatBar->SetupInitialHealth(ASC);
}

void UUW_HUD::UpdateCurrentHealth(float InNewCurrentHealth)
{
	PlayerStatBar->SetCurrentHealth(InNewCurrentHealth);
}

void UUW_HUD::UpdateMaximumHealth(float InNewMaxHealth)
{
	PlayerStatBar->SetMaximumHealth(InNewMaxHealth);
}
