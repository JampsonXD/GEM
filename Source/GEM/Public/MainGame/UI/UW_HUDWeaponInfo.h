// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MainGame/GEMWeaponData.h"
#include "UW_HUDWeaponInfo.generated.h"

/**
 * 
 */
UCLASS()
class GEM_API UUW_HUDWeaponInfo : public UUserWidget
{
	GENERATED_BODY()

	public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon | UI")
	void SetWeaponInfoFromWeaponData(UGEMWeaponData* InWeaponData);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon | UI")
	void SetWeaponName(FName InWeaponName);

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon | UI")
	void SetWeaponCurrentAmmo(int InCurrentAmmo);

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon | UI")
	void SetWeaponMaximumAmmo(int InMaximumAmmo);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Weapon | UI")
	void SetWeaponReserveAmmoText(int InReserveAmmo);

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* WeaponName_Text;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* WeaponAmmo_Text;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon | UI")
	int32 CurrentWeaponAmmo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon | UI")
	int32 MaxWeaponAmmo;
};
