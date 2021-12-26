// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGame/GEMItemData.h"

FPrimaryAssetId UGEMItemData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(ItemType, GetFName());
}
