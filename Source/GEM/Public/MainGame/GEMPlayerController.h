// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GEMCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/UW_HUD.h"
#include "UI/UW_RespawnTimer.h"
#include "GEMPlayerController.generated.h"

class AGEMWeapon;
/**
 * 
 */
UCLASS()
class GEM_API AGEMPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AGEMPlayerController();
	
	// Setter for our Spawn Transform, only set on server
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Spawn")
	void SetSpawnTransform(FTransform Transform);

	// Getter for our Spawn Transform
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player Spawn")
	FTransform GetSpawnTransform() const;

	// Respawn Timer
	UFUNCTION(Client, Reliable, WithValidation, Category = "Player Respawn")
	void SetRespawnCountdown(float TimeRemaining);
	void SetRespawnCountdown_Implementation(float TimeRemaining);
	bool SetRespawnCountdown_Validate(float TimeRemaining);

	UFUNCTION(Client, Reliable, WithValidation, Category = "Player Respawn")
	void StopRespawnCountdown();
	void StopRespawnCountdown_Implementation();
	bool StopRespawnCountdown_Validate();

	// Called when we kill another player
	UFUNCTION(BlueprintImplementableEvent, Category = "GEM GAS | Death")
	void KilledCharacter(AGEMCharacter* InKilledPlayer, float InCharacterLevel);

	// Called when another player or thing kills us, can be ourselves if we die from fall damage or other natural deaths
	UFUNCTION(BlueprintImplementableEvent, Category = "GEM GAS | Death")
	void WasKilled(AGEMCharacter* InKillingPlayer, bool bSelfInflicted);
	
protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Our default player HUD class to spawn
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UUW_HUD> HUDClass;

	// Pointer to our HUD
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	UUW_HUD* HUD;

public:
	// Creates our Clients HUD and adds it to the player viewport
	UFUNCTION()
	void CreateHUD();

	// Getter for our HUD
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HUD")
	UUW_HUD* GetGEMHUD();

protected:

	virtual void OnRep_Pawn() override;
	
	// Set all of our Weapon Data using a reference to a weapon
	UFUNCTION(Category = "HUD | Weapons")
	void SetHUDWeaponData(AGEMWeapon* InWeapon);

public:
	// Hides or Shows the HUD Weapon Area
	UFUNCTION(Category = "HUD | Weapons")
	void SetHUDWeaponVisibility(bool bShow);

	UFUNCTION(Category = "HUD | Weapons")
	void SetWeaponInfoFromWeaponData(UGEMWeaponData* InWeaponData);

	UFUNCTION(Category = "HUD | Ammo")
	void SetAmmoInfoFromAmmoData(UGEMAmmoData* InAmmoData);
	
	// Set our Weapon Name
	UFUNCTION(Category = "HUD | Weapons")
	void SetHUDWeaponName(FName InWeaponName);

	// Set Weapon Current Ammo Text
	UFUNCTION(Category = "HUD | Weapons")
	void SetCurrentAmmoText(int InCurrentAmmo);

	// Set Weapon Maximum Ammo Text
	UFUNCTION(Category = "HUD | Weapons")
	void SetMaximumAmmoText(int InMaximumAmmo);

protected:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player Spawn")
	FTransform SpawnTransform;

	// Widget Class for our Respawn Widget
	UPROPERTY(EditDefaultsOnly, Category = "Player Respawn")
	TSubclassOf<UUW_RespawnTimer> RespawnWidgetClass;

	// Our Actual Widget
	UPROPERTY(EditDefaultsOnly, Category = "Player Respawn")
	UUW_RespawnTimer* RespawnWidget;
};
