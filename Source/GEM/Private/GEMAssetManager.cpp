// Fill out your copyright notice in the Description page of Project Settings.


#include "GEMAssetManager.h"
#include "AbilitySystemGlobals.h"

// Defining Primary Asset Types
const FPrimaryAssetType UGEMAssetManager::WeaponType = TEXT("Weapon");
const FPrimaryAssetType UGEMAssetManager::KeyType = TEXT("Key");
const FPrimaryAssetType UGEMAssetManager::AmmoType = TEXT("Ammo");
const FPrimaryAssetType UGEMAssetManager::ScopeType = TEXT("Scope");
const FPrimaryAssetType UGEMAssetManager::CharacterType = TEXT("Character");

void UGEMAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}

UGEMAssetManager& UGEMAssetManager::Get()
{
	UGEMAssetManager* This = Cast<UGEMAssetManager>(GEngine->AssetManager);

	if(!This)
	{
		UE_LOG(LogClass, Fatal, TEXT("Invalid Asset Manager, must use a UGEMAssetManager"))
		return *NewObject<UGEMAssetManager>();
	}
	
	return *This;
}
