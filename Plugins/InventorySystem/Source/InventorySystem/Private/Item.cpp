// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "ItemTypes.h"

UItem::UItem(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsStackable = false;
	MaxStackCount = -1;
	CurrentStackCount = 0;
	ItemImage = nullptr;
}

void UItem::Use()
{
	
}

bool UItem::IsStackable()
{
	return bIsStackable;
}

bool UItem::HasUnlimitedStacks()
{
	return MaxStackCount == -1;
}

int32 UItem::GetMaxItemStacks()
{
	return MaxStackCount;
}

int32 UItem::GetCurrentItemStacks()
{
	return CurrentStackCount;
}

FItemUIData UItem::MakeItemData()
{
	FItemUIData ItemData = FItemUIData(ItemName, ItemDescription, ItemImage, bIsStackable, CurrentStackCount, MaxStackCount);
	return ItemData;
}

FOnCurrentStackCountChanged& UItem::GetOnCurrentStackCountChangedDelegate()
{
	return OnCurrentStackCountChanged;
}

void UItem::AddStacks(int32 StacksToAdd)
{
	const int32 OldStackCount = CurrentStackCount;

	// We don't need to check for our Max Stack Count, the Inventory System Component will do that for us
	CurrentStackCount += StacksToAdd;

	// Broadcast that our stack count has changed
	if(OnCurrentStackCountChanged.IsBound())
	{
		OnCurrentStackCountChanged.Broadcast(OldStackCount, CurrentStackCount);
	}
}
