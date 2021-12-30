// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

UItem::UItem(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsStackable = false;
	MaxStackCount = -1;
	CurrentStackCount = 0;
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

void UItem::AddStacks(int32 Amount)
{
	// Check if we have unlimited stacks and if so, we don't need to worry about max stack count
	if(HasUnlimitedStacks())
	{
		CurrentStackCount = CurrentStackCount + Amount;
		return;
	}
	CurrentStackCount = FMath::Min(CurrentStackCount + Amount, MaxStackCount);
}
