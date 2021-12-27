// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGame/DataAssets/QuestDataAsset.h"

#include "GEMAssetManager.h"

FPrimaryAssetId UQuestDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(UGEMAssetManager::QuestType, GetFName());
}
