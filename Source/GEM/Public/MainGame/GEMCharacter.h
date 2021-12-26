// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GEMTypes.h"
#include "AbilitySystemInterface.h"
#include "EnhancedInputSubsystemInterface.h"
#include "GEMInventoryTypes.h"
#include "DataAssets/GEMWeaponData.h"
#include "GameFramework/Character.h"
#include "GAS/GEMAbilitySystemComponent.h"
#include "GAS/GEMAttributeSetBase.h"
#include "GAS/GEMGameplayAbility.h"
#include "InputMappingContext.h"
#include "UI/GEMPopupDamageWC.h"
#include "GEMCharacter.generated.h"

class AGEMWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHitReactPlayed, const EHitReactDirection, HitReactDirection);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponChanged, const AGEMWeapon*, NewWeapon);


UCLASS(config=Game)
class AGEMCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:

	/** First Person Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCamera;

	/** First Person Mesh **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Info")
	FPrimaryAssetId CharaterInfo;


public:
	
	AGEMCharacter(const class FObjectInitializer& ObjectInitializer);

	// Get our First Person Mesh
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Mesh")
	USkeletalMeshComponent* GetFirstPersonMesh();

	// Get our Character Information AssetID
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character Info")
	FPrimaryAssetId GetCharacterInfo() const;


	// Get ASC
	UFUNCTION(BlueprintCallable)
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// Hit React Delegate
	UPROPERTY(BlueprintAssignable)
	FHitReactPlayed ShowHitReact;

	// ============================================================================= //
	// ==============               Damage Popup              ====================== //
	// ============================================================================= //

protected:
	// Our queue of DamagePopups to play
	TArray<FDamagePopupData> DamagePopupQueue;

	// Damage Popup Timer Handle
	FTimerHandle DamagePopupTimerHandle;

	// How long we should wait before spawning and adding a new damage popup
	UPROPERTY(EditDefaultsOnly, Category = "Damage Popup")
	float DamagePopupDelayRate;

	// How long we should wait before spawning and adding a new damage popup
	UPROPERTY(EditDefaultsOnly, Category = "Damage Popup")
	TSubclassOf<UGEMPopupDamageWC> DamagePopupClass;

	// Creates our Widget and displays the Damage Popup
	UFUNCTION()
	void ShowDamagePopup();
public:
	// Adds a Damage Popup Value to the characters Popup Queue;
	// @PopupData passes in Damage and Location values to add to our Damage Array
	UFUNCTION(Client, Unreliable, Category = "Damage Popup")
	void AddDamagePopupToQueue(FDamagePopupData PopupData, FGameplayTagContainer BonusEffectTags);
	
	// Death functionality
	UFUNCTION(BlueprintNativeEvent, Category = "GEM GAS | Death")
	void Execute_Death();

	// Called after Execute_Death to start respawning the player
	UFUNCTION(BlueprintCallable, Category = "GEM GAS | Death")
	void FinishDeath();

	// Checks if the player is still currently alive
	UFUNCTION(BlueprintCallable, Category = "GEM GAS | Death")
	bool IsAlive();

	// Ease of use functions for getting Attribute values
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GEM GAS | Attributes")
	float GetMovementSpeed() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GEM GAS | Attributes")
	float GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GEM GAS | Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GEM GAS | Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GEM GAS | Ammo Attributes")
	float GetRifleReserveAmmo() const;

	void SetHealth(float InHealthValue);

	UFUNCTION(BlueprintCallable, Category = "Animations")
	EStanceType GetCurrentStance();
	
	UFUNCTION(BlueprintCallable, Category = "Animations")
	void SetCurrentStance(EStanceType InType);

protected:

	virtual void PossessedBy(AController* NewController) override;
	
	virtual void FellOutOfWorld(const UDamageType& dmgType) override;

	virtual void PawnClientRestart() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Fell out of World")
	void FellOffWorld();

	virtual void OnRep_PlayerState() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// Networking
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Weak Object References to our Ability System Component and Attribute Set
	TWeakObjectPtr<class UGEMAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UGEMAttributeSetBase> AttributeSet;
	TWeakObjectPtr<class UGEMAttributeSetAmmo> AmmoAttributeSet;

	// ============================================================================= //
	// ==============               GAS Defaults              ====================== //
	// ============================================================================= //

	// Stats the character will be initialized with
	UPROPERTY(EditDefaultsOnly, Category = "Starting Effects")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	// Abilities the character will be initialized with
	UPROPERTY(EditDefaultsOnly, Category = "Starting Abilities")
	TArray<TSubclassOf<class UGEMGameplayAbility>> DefaultAbilities;

	UFUNCTION()
	void InitCharacterStats();
	
	UFUNCTION()
	void GrantDefaultAbilities();

	UFUNCTION()
	void BindASCInput();

	// Check for if our ASC has its input bound
	bool ASCInputBound = false;

	// ============================================================================= //
	// ==============			      Inventory			     ====================== //
	// ============================================================================= //

protected:

	// Our Characters Inventory
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
	FCharacterInventory CharacterInventory;

	// Default Weapon to Load in
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FPrimaryAssetId DefaultWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon, BlueprintReadWrite, Category = "Inventory | Weapon")
	AGEMWeapon* CurrentWeapon;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponChanged OnWeaponChanged;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
	FName WeaponAttachPoint;

	// Called after our asynchronous load of our default weapon; Adds the weapon to our player inventory
	UFUNCTION()
	void OnDefaultWeaponLoaded(FPrimaryAssetId IDLoaded);

public:

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AGEMWeapon* GetCurrentWeapon();
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddWeaponToInventory(FInventoryWeaponData NewWeaponData, bool bEquipWeapon = false);

	UFUNCTION()
	FName GetWeaponAttachPoint();

	// Searches through our Weapons to find the first value with the same UGEMWeaponData, and updates its Ammo Count; Bool = FoundMatch
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory | Helpers")
	bool FindAndUpdateWeaponAmmo(FInventoryWeaponData& InInventoryWeapon);

	// Current Weapon Ammo Changed
	UFUNCTION()
	void CurrentWeaponAmmoChanged(int32 OldAmmoCount, int32 NewAmmoCount);

	UFUNCTION()
	void MaximumWeaponAmmoChanged(int32 OldAmmoCount, int32 NewAmmoCount);

protected:
	
	UFUNCTION()
	void SetCurrentWeapon(AGEMWeapon* NewWeapon, AGEMWeapon* OldWeapon);

	UFUNCTION()
	void UnequipWeapon(AGEMWeapon* Weapon);
	
	UFUNCTION()
	void OnRep_CurrentWeapon(AGEMWeapon* PreviousWeapon);

	UFUNCTION(Category = "Inventory | Spawning")
	AGEMWeapon* SpawnWeaponDeferred(FInventoryWeaponData WeaponData);

public:
	// Tries to equip the next weapon in our Inventory Weapons Array; Returns False if no slot was found to equip
	UFUNCTION(BlueprintCallable, Category = "Inventory | Weapon")
	bool EquipNextWeapon();

	// Tries to equip the next weapon in our Inventory Weapons Array; Returns False if no slot was found to equip
	UFUNCTION(BlueprintCallable, Category = "Inventory | Weapon")
	bool EquipLastWeapon();

	// Equip the weapon at the specified index
	UFUNCTION()
	void EquipWeaponAtIndex(int32 WeaponIndex);

protected:
	// Try to find our current Weapon Index
	UFUNCTION()
	bool FindCurrentWeaponIndex(int32& InIndex);
	
	// ============================================================================= //
	// ==============             Animations and Input        ====================== //
	// ============================================================================= //

	// Current Stance our Character is in
	UPROPERTY(Replicated,BlueprintReadWrite, Category = "Animations")
	EStanceType CurrentStance;

	FGameplayTag CurrentWeaponTag;

	FGameplayTag NoWeaponTag;
	
	// Death Montage
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* DeathMontage;

	// Movement Input Action
	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	FInputActions MovementInputAction;

	// Rotation/Turn Input Action
	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	FInputActions RotateInputAction;
	
	// Jump Input Action
	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	FInputActions JumpInputAction;
	
	// Default Input Mapping Context
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Player Input")
	UInputMappingContext* DefaultMappingContext;

	// Hit React Tags
	FGameplayTag HitReactFront;
	FGameplayTag HitReactBack;
	FGameplayTag HitReactRight;
	FGameplayTag HitReactLeft;

	// Headshot Tag
	FGameplayTag Headshot;

	UFUNCTION()
	void MoveCharacter(const FInputActionValue& ActionValue);

	UFUNCTION()
	void RotateCharacter(const FInputActionValue& ActionValue);

public:
	// Get the hit react direction from a HitResult
	UFUNCTION()
	EHitReactDirection GetHitReactDirection(const FVector& ImpactPoint) const;
	
	// Broadcast our hit react
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	virtual void PlayHitReact(FGameplayTag HitDirection);
	virtual void PlayHitReact_Implementation(FGameplayTag HitDirection);
	virtual bool PlayHitReact_Validate(FGameplayTag HitDirection);

public:
	/** Returns the First Person Camera subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCamera() const {return FirstPersonCamera;}
};

