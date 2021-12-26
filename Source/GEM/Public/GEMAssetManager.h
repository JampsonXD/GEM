// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GEMAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class GEM_API UGEMAssetManager : public UAssetManager
{
	GENERATED_BODY()

	public:

	virtual void StartInitialLoading() override;

	virtual UGEMAssetManager& Get();

	// Primary Asset Types
	static const FPrimaryAssetType WeaponType;
	static const FPrimaryAssetType KeyType;
	static const FPrimaryAssetType AmmoType;
	static const FPrimaryAssetType ScopeType;
	static const FPrimaryAssetType CharacterType;
};
