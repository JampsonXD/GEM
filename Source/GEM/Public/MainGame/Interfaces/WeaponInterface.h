// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainGame/GEMWeapon.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GEM_API IWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Weapon Interface")
	bool CanReload();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Weapon Interface")
	AGEMWeapon* GetWeapon();
	
};
