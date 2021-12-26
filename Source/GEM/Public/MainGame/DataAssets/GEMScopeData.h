// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GEMItemData.h"
#include "MainGame/GAS/GEMGameplayAbility.h"
#include "GEMScopeData.generated.h"

/**
 * 
 */
UCLASS()
class GEM_API UGEMScopeData : public UGEMItemData
{
	GENERATED_BODY()

public:

	UGEMScopeData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scope")
	bool bUsesSceneCapture;
	
	// Abilities Granted from our Scope
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scope | Abilities")
	TArray<TSubclassOf<UGEMGameplayAbility>> ScopeAbilities;

	// Array of ScopeMeshData, needs to be an array for checks if we need to add iron sight front and back, etc.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scope | Appearance")
	TArray<FSubWeaponMeshData> ScopeMeshData;

	// Players FOV when using this scope aimed in
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scope | Stats")
	float PlayerADSFOV;

	// The scopes FOV when using scope aimed in
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scope | Stats")
	float ScopeADSFOV;

	// Camera Offset to add to our players mesh when aimed in
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scope | Stats")
	float CameraOffset;
	
};
