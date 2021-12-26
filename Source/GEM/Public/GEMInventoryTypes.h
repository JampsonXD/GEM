// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainGame/DataAssets/GEMWeaponData.h"
#include "GEMInventoryTypes.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct GEM_API FInventoryWeaponData
{
	GENERATED_BODY()

	// Default Constructor
	FInventoryWeaponData()
	{
		WeaponData = nullptr;
		WeaponPointer = nullptr;
		Ammo = -1;
	}

	// Couple of constructors for setting weapon to full ammo capacity or returning a weapon with a specific ammo count
	
	FInventoryWeaponData(UGEMWeaponData InWeaponData)
	{
		WeaponData = &InWeaponData;
		WeaponPointer = nullptr;
		Ammo = WeaponData->AmmoCapacity;
	}

	FInventoryWeaponData(UGEMWeaponData* InWeaponData)
	{
		WeaponData = InWeaponData;
		WeaponPointer = nullptr;
		Ammo = WeaponData->AmmoCapacity;
	}

	FInventoryWeaponData(UGEMWeaponData& InWeaponData, int& InAmmoAmount)
	{
		WeaponData = &InWeaponData;
		WeaponPointer = nullptr;
		Ammo = InAmmoAmount;
	}
	
	// Pointer to our WeaponData Object
	UPROPERTY(BlueprintReadWrite, Category = "Inventory Weapon Data")
	UGEMWeaponData* WeaponData;

	// Pointer to our Weapon Object, used to identify what the current weapon index is
	UPROPERTY(BlueprintReadWrite, Category = "Inventory Weapon Data")
	AGEMWeapon* WeaponPointer;
	
	// How much additional ammunition/charges we have left over since last using the weapon
	UPROPERTY(BlueprintReadWrite, Category = "Inventory Weapon Data")
	int Ammo;
	
};

USTRUCT(BlueprintType)
struct GEM_API FCharacterInventory
{
	GENERATED_BODY()

	// Our Array of Weapons
	UPROPERTY(BlueprintReadWrite, Category = "Character Inventory")
	TArray<FInventoryWeaponData> Weapons;

	// TODO : Add extra slots for various other items besides weapons, such as keys, consumables, etc
};
