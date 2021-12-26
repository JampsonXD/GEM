// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GEMCharacter.h"
#include "DataAssets/GEMScopeData.h"
#include "GameFramework/Actor.h"
#include "GAS/GEMAbilitySystemComponent.h"
#include "GAS/GEMGameplayAbility.h"
#include "GEMWeapon.generated.h"

// OnRep Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAmmoChangedSignature, int32, OldAmmoCount, int32, NewAmmoCount);

UCLASS()
class GEM_API AGEMWeapon : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AGEMWeapon();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Getters for our Mesh Components**/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Mesh")
	virtual USkeletalMeshComponent* GetSkeletalMeshFirstPerson() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Attatchments | Mesh")
	USkeletalMeshComponent* GetStockMesh() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Attatchments | Mesh")
	UStaticMeshComponent* GetHandguardMesh() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Attatchments | Mesh")
	UStaticMeshComponent* GetIronsightFrontMesh() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Attatchments | Mesh")
	UStaticMeshComponent* GetIronsightBackMesh() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Attatchments | Mesh")
	USkeletalMeshComponent* GetSight() const;

	/** End of Getters for our Mesh Components**/

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Attatchments | Scene Capturer")
	USceneCaptureComponent2D* GetScopeSceneCapture() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon | Scope")
	void ToggleWeaponScope(bool bUse);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Scope")
	bool UsesScopeSceneCapture() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Scope")
	float GetScopeFOVChange() const;

	UPROPERTY(BlueprintAssignable)
	FAmmoChangedSignature CurrentAmmoChanged;

	UPROPERTY(BlueprintAssignable)
	FAmmoChangedSignature MaxAmmoChanged;

	UFUNCTION(BlueprintCallable, Category = "Weapon | Ammo")
	void SetCurrentAmmo(int NewAmmoAmount);

	UFUNCTION(BlueprintCallable, Category = "Weapon | Ammo")
	void SetMaximumAmmo(int NewAmmoAmount);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Ammo")
	int GetCurrentAmmo() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Ammo")
	int GetMaximumAmmo() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Recoil")
	float GetRecoilHorizontal() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Aim")
	float GetRecoilVertical() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Aim")
	float GetScopeADSFOV() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Aim")
	float GetPlayerADSFOV() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Aim")
	float GetADSCameraOffset() const;

	UFUNCTION()
	FGameplayTag GetWeaponTag() const;

	/** Getters for our Weapon Data **/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Data")
	UGEMWeaponData* GetWeaponData() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Effects")
	FEffectsPack GetOnFiredEffects() const;

	// Returns the proper Impact Effect based on the passed in hit result
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Effects")
	FEffectsPack GetBulletImpactEffects(FHitResult InHitResult) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Effects")
	UParticleSystem* GetWeaponTrailEffect() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Effects")
	TSubclassOf<UCameraShakeBase> GetOnFireCameraShake() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Effects")
	UAnimMontage* ReloadMontage() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Stats")
	float GetWeaponRange() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon | Stats")
	float GetWeaponDamage() const;

	/** End of Getters for our Weapon Data **/

	/** Getters for our Ammo Data **/

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ammo | Data")
	UGEMAmmoData* GetAmmoData() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ammo | Data")
	UGEMScopeData* GetScopeData() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ammo | Data")
	TSubclassOf<UGameplayEffect> GetAmmoConsumeClass() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ammo | Data")
	FGameplayAttribute GetAmmoAttribute() const;

protected:

	// Components

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* CollisionComponent;

	// Main Weapon Mesh
	UPROPERTY(VisibleAnywhere, Category = "Weapon | Mesh")
	USkeletalMeshComponent* WeaponMeshFirstPerson;

	// Stock Attachment
	UPROPERTY(VisibleAnywhere, Category = "Weapon | Attatchment")
	USkeletalMeshComponent* StockMesh;

	// Irontsight near the player
	UPROPERTY(VisibleAnywhere, Category = "Weapon | Attatchment")
	UStaticMeshComponent* HandguardMesh;
	
	// Irontsight near the player
	UPROPERTY(VisibleAnywhere, Category = "Weapon | Attatchment")
	UStaticMeshComponent* IronsightFrontMesh;

	// Ironsight farther down the weapon
	UPROPERTY(VisibleAnywhere, Category = "Weapon | Attatchment")
	UStaticMeshComponent* IronsightBackMesh;

	// General Sight for our weapon
	UPROPERTY(VisibleAnywhere, Category = "Weapon | Attachment")
	USkeletalMeshComponent* SightMesh;

	// Scene Capture used for some scopes
	UPROPERTY(VisibleAnywhere, Category = "Weapon | Attachment")
	USceneCaptureComponent2D* ScopeSceneCapturer;

	// Mesh setup from SubWeaponMeshData
	UFUNCTION()
	static void SetupSkeletalAttachmentFromSubWeaponMeshData(USkeletalMeshComponent* InMeshComponent, const FSubWeaponMeshData& InMeshData, USkeletalMeshComponent* ParentMesh);

	UFUNCTION()
	static void SetupStaticAttachmentFromSubWeaponMeshData(UStaticMeshComponent* InMeshComponent, const FSubWeaponMeshData& InMeshData, USkeletalMeshComponent* ParentMesh);
	
	
	// Replication stuff
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;

	FGameplayTag WeaponFiringTag;

	// ============================================================================= //
	// ==============               GAS Defaults              ====================== //
	// ============================================================================= //
	
	// ASC
	UPROPERTY()
	UGEMAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Granted Abilities")
	// Granted Abilities while we have this weapon equipped, passed in from our data asset
	TArray<TSubclassOf<UGEMGameplayAbility>> GrantedAbilities;

	// Our Ability Spec Handles for our abilities
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
	
	public:
	
	// Add our GEMGameplayAbilities
	UFUNCTION()
	void AddAbilities();

	// Remove our GEMGameplayAbilities
	UFUNCTION()
	void RemoveAbilities();

	UFUNCTION(BlueprintCallable, Category = "Weapon | GAS")
	int GetWeaponLevel();

	// ============================================================================= //
	// ==============               Equipping              ====================== //
	// ============================================================================= //

	// Equip Weapon
	UFUNCTION()
	void EquipWeapon();

	UFUNCTION()
	void UnequipWeapon();

	protected:

	// Owning Character
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Weapon")
	AGEMCharacter* OwningCharacter;

	public:
	// Set our owning character, will be called on the player character
	UFUNCTION()
	void SetOwningCharacter(AGEMCharacter* InCharacter);

	UFUNCTION()
	void SetupData(const FInventoryWeaponData WeaponData);

	UFUNCTION()
	void SetScopeData(const UGEMScopeData* OldScopeData, UGEMScopeData* NewScopeData);

	// Set Client Data Asset, called from server
	UFUNCTION(Client, Reliable, Category = "Setup")
	void Client_SetDataAsset(UGEMWeaponData* InWeaponData);

	// Called to set values such as our Skeletal Mesh 
	UFUNCTION()
	void SetValuesFromWeaponData();

	// Called at the end of SetValuesFromWeaponData, for additional setup in BP
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon | Setup")
	void PostSetupWeaponData();

protected:
	// The DataAsset that created us
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon | Data")
	UGEMWeaponData* WeaponData;

	// Our Ammo Data Asset
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon | Data")
	UGEMAmmoData* AmmoData;

	// Our Scope Data Asset
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon | Data")
	UGEMScopeData* ScopeData;
	
	// Current Ammo Count
	UPROPERTY(ReplicatedUsing = OnRep_CurrentAmmoCount, BlueprintReadOnly, EditAnywhere, Category = "Weapon | Ammo")
	int32 CurrentAmmoCount;

	// Maximum Ammo in Magazine
	UPROPERTY(ReplicatedUsing = OnRep_MaxAmmoCount,BlueprintReadOnly, EditAnywhere, Category = "Weapon | Ammo")
	int32 MaxAmmoCount;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Weapon | Fire Rate")
	int32 FireRate;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Weapon | Recoil")
	float RecoilHorizontal;

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Weapon | Recoil")
	float RecoilVertical;

	// How much we will adjust our Scopes FOV when Aiming Down Sights
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Weapon | Aim")
	float ScopeADSFOV;

	// How much our own player will adjust his FOV when Aiming Down Sights
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Weapon | Aim")
	float PlayerADSFOV;

	// Camera offset to use for player when ADS
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere, Category = "Weapon | Aim")
	float ADSCameraOffset;
	
	UFUNCTION()
	virtual void OnRep_CurrentAmmoCount(int32 OldCurrentAmmoCount);

	UFUNCTION()
	virtual void OnRep_MaxAmmoCount(int32 OldMaxAmmoCount);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
