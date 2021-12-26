// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GEMAmmoData.h"
#include "MainGame/GEMItemData.h"
#include "GEMTypes.h"
#include "DataAssets/GEMScopeData.h"
#include "GAS/GEMGameplayAbility.h"
#include "GEMWeaponData.generated.h"

class AGEMWeapon;


/**
 * 
 */
UCLASS()
class GEM_API UGEMWeaponData : public UGEMItemData
{
	GENERATED_BODY()

public:

	UGEMWeaponData();

	// Weapon Tag, primarily used for Animations
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	FGameplayTag WeaponTag;

	// Gameplay Abilities we will grant when this weapon is equipped
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TArray<TSubclassOf<UGEMGameplayAbility>> WeaponAbilities;

	// The weapon class we will spawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<AGEMWeapon> WeaponClass;

	// Our Ammo Asset that the weapon uses by default
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	UGEMAmmoData* DefaultAmmoDataAsset;

	// Our Scope Asset that the weapon uses by default
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	UGEMScopeData* DefaultScopeDataAsset;
	
    // Animation blueprint we can use for our weapon
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Animation")
    TSubclassOf<UAnimInstance> WeaponAnimationBlueprint;
    
    // Reload Montage
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Animation")
    UAnimMontage* ReloadMontage;

	// Mesh for our Weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Appearance")
	FWeaponMeshData WeaponMesh;

	// Child Skeletal Meshes to Attach
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Appearance")
	TArray<FSubWeaponMeshData> AttachmentMeshes;

	// Effects for our weapon firing, these are our muzzle effects that come off the gun
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Effects")
	FEffectsPack FireEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Effects")
	FImpactEffects ImpactEffects;

	// Our Weapon Trail Effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Effects")
	UParticleSystem* WeaponTrail;

	// Camera Shake Played on Fire
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Effects")
	TSubclassOf<UCameraShakeBase> OnFireCameraShake;

	// Texture we will use for our Weapon Image for our HUD Class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | HUD")
	UTexture2D* WeaponHUDImage;

	// Texture for our Crosshair
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | HUD")
	UTexture2D* WeaponCrosshair;
	
	// Maximum amount of ammo we can have stored on this weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Stats")
	float AmmoCapacity;

	// Fire rate of our weapon, use RPM for firearms, direct cooldown for Melee weapons
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Stats")
	float FireRate;

	// How far away our weapon can fire from before the bullet cannot go further, the effective range of the weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Stats")
	float EffectiveRange;

	// Damage our weapon can do per each instance (In the future, things like shotguns might have more than one instance of damage)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Stats")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Stats")
	float HorizontalRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Stats")
	float VerticalRecoil;
	
};
