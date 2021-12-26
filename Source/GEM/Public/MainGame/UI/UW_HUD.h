// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UW_DeathRecap.h"
#include "UW_ExperiencePopup.h"
#include "UW_HUDWeaponInfo.h"
#include "UW_PlayerStatBar.h"
#include "Blueprint/UserWidget.h"
#include "Components/Overlay.h"
#include "MainGame/DataAssets/GEMWeaponData.h"
#include "UW_HUD.generated.h"

/**
 * 
 */
UCLASS()
class GEM_API UUW_HUD : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = "HUD | Weapon")
	void SetWeaponDataInfo(UGEMWeaponData* InWeaponData);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD | Ammo")
	void SetAmmoDataInfo(UGEMAmmoData* InAmmoData);
	
	UFUNCTION()
	void SetWeaponName(FName InWeaponName);

	UFUNCTION()
	void SetWeaponCurrentAmmoText(int InCurrentAmmoCount);

	UFUNCTION()
	void SetWeaponMaximumAmmoText(int InMaximumAmmoCount);

	UFUNCTION(BlueprintCallable, Category = "HUD | Weapon")
	void SetWeaponReserveAmmoText(int InReserveAmmoCount);

	UFUNCTION()
	void ToggleWeaponInfoVisibility(ESlateVisibility InVisibility);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD | Weapon")
	void SetWeaponCrosshair(UTexture2D* InCrosshairTexture);

	UFUNCTION(BlueprintCallable, Category = "HUD | Death")
	void PlayDeathRecap(UGEMCharacterData* InEnemyCharacterData);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD | Death")
	void StartRespawn();

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD | Death")
	void EndRespawn();

	UFUNCTION(BlueprintCallable, Category = "HUD | Experience Popup")
	void AddExperiencePopup(float InExperienceGained);
	
protected:

	UFUNCTION(BlueprintCallable, Category = "HUD | Setup")
	void SetupInitialHealth(UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "HUD | Health")
	void UpdateCurrentHealth(float InNewCurrentHealth);

	UFUNCTION(BlueprintCallable, Category = "HUD | Health")
	void UpdateMaximumHealth(float InNewMaxHealth);
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUW_HUDWeaponInfo* WeaponInfo;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUW_PlayerStatBar* PlayerStatBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UUW_DeathRecap* PlayerDeathRecap;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* ExperienceOverlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UUW_ExperiencePopup> ExperiencePopupClass;
};
