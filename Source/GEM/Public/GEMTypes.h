// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "GEMTypes.generated.h"

static FGameplayTag DeadTag = FGameplayTag::RequestGameplayTag("States.Dead");
static FGameplayTag InCombatTag = FGameplayTag::RequestGameplayTag("States.InCombat");


UENUM(BlueprintType)
enum class EWeaponAttachmentSlot : uint8
{
	None,
	Sight,
	Grip,
	Stock,
	IronsightFront,
	IronsightBack,
	Handguard
};

UENUM(BlueprintType)
enum class EScopeAttachmentType : uint8
{
	None,
	Sight,
	IronSightFront,
	IronSightBack
};

USTRUCT(BlueprintType)
struct FDamagePopupData
{
	GENERATED_BODY()

	FDamagePopupData()
	{
		Location = FVector(0.f,0.f,0.f);
		DamageAmount = 0.f;
	}

	FDamagePopupData(FVector InLocation, float InDamageAmount)
	{
		Location = InLocation;
		DamageAmount = InDamageAmount;
		Tags = FGameplayTagContainer::EmptyContainer;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Popup Data")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Popup Data")
	float DamageAmount;

	FGameplayTagContainer Tags;
	
};


// Contains a set of SFX and VFX
USTRUCT(BlueprintType)
struct FEffectsPack
{
	GENERATED_BODY()

	// Default to null
	FEffectsPack()
	{
		CascadeEffects = nullptr;
		NiagaraEffect = nullptr;
		SoundEffect = nullptr;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "EffectsPack")
	UParticleSystem* CascadeEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "EffectsPack")
	UNiagaraSystem* NiagaraEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectsPack")
	USoundCue* SoundEffect;
};

// Contains different FEffectsPacks for different Impact Effects
USTRUCT(BlueprintType)
struct FImpactEffects
{
	GENERATED_BODY()

	FImpactEffects()
	{
		PlayerHit =  FEffectsPack();
		WorldHit = FEffectsPack();
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectsPack | Impact Effects")
	FEffectsPack PlayerHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectsPack | Impact Effects")
	FEffectsPack WorldHit;
	
};

// Stores offsets, skeletal mesh, other things needed to position our meshes properly
USTRUCT(BlueprintType)
struct FWeaponMeshData
{
	GENERATED_BODY()

	FWeaponMeshData()
	{
		SkeletalMesh = nullptr;
		LocationOffset = FVector(0.f,0.f,0.f);
		RotationOffset = FRotator(0.f,0.f,0.f);
		SocketAttachName = FName("None");
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Data")
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Data")
	FVector LocationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Data")
	FRotator RotationOffset;

	// Name of the socket we should attach to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Data")
	FName SocketAttachName;
	
};

USTRUCT(BlueprintType)
struct FSubWeaponMeshData : public FWeaponMeshData
{
	GENERATED_BODY()

	FSubWeaponMeshData()
	{
		Super();
		StaticMesh = nullptr;
		AttachmentSlot = EWeaponAttachmentSlot::None;
	}

	// Might use a Static Mesh instead of our Skeletal Mesh depending on the situation, add a pointer to a static mesh just incase
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Data")
	UStaticMesh* StaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Data")
	EWeaponAttachmentSlot AttachmentSlot;
};

// GAS Input Binding Enum
UENUM(BlueprintType)
enum class EAbilityInput : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),
	// 1 Confirm
	Confirm			UMETA(DisplayName = "Confirm"),
	// 2 Cancel
	Cancel			UMETA(DisplayName = "Cancel"),
	// 3 LMB
	Ability1		UMETA(DisplayName = "Ability1"),
	// 4 RMB
	Ability2		UMETA(DisplayName = "Ability2"),
	// 5 Q
	Ability3		UMETA(DisplayName = "Ability3"),
	// 6 E
	Ability4		UMETA(DisplayName = "Ability4"),
	// 7 R
	Ability5		UMETA(DisplayName = "Ability5"),
	// 8 Sprint
	Sprint			UMETA(DisplayName = "Sprint"),
	// 9 Jump
	Jump			UMETA(DisplayName = "Jump")
};

UENUM(BlueprintType)
enum EHitReactDirection
{
	None,
	Front,
	Back,
	Right,
	Left
};

USTRUCT(BlueprintType)
struct FInputActions
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction;
};


// Player Data Structure
USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
	FName AvatarRowHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
	FName PlayerName;

	FPlayerData()
	{
		AvatarRowHandle = FName("Default");
		PlayerName = FName("Default");
	}
	
};

UENUM(BlueprintType)
enum class EStanceType : uint8
{
	OutOfCombat UMETA(DisplayName = "Out of Combat"),
	MageCombat UMETA(DisplayName = "Mage Combat")
};