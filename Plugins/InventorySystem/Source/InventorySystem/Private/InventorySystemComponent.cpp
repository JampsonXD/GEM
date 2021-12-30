// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystemComponent.h"


// Sets default values for this component's properties
UInventorySystemComponent::UInventorySystemComponent(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Initialize as Unlimited Inventory Size
	MaxInventorySize = -1;

	
	PrimaryComponentTick.bCanEverTick = true;
	
}

bool UInventorySystemComponent::AddItem(TSubclassOf<UItem> Item, int32 Amount)
{
	// Make sure we are actually adding an amount or have a valid Item Class
	if(Amount <= 0 || !IsValid(Item))
	{
		return false;
	}
	
	TArray<UItem*> ValidItems;
	bool bAddedItems = false;

	// Check if we are a stackable item and already have items of the same type
	if(Cast<UItem>(Item->GetDefaultObject())->IsStackable() && ContainsValidItemOfClass(Item,ValidItems))
	{
		bAddedItems = true;
		AddStacksToItems(ValidItems, Amount);
	}
	
	// We have no stacks to update left or we have no inventory space
	if(Amount <= 0 || IsInventoryFull())
	{
		return bAddedItems;
	}
	
	bAddedItems = true;
	
	// Continue creating items from class until we have no stacks to give left or our inventory is full
	while(!IsInventoryFull() && Amount > 0)
	{
		// Create a new Item Instance
		UItem* NewItem = NewObject<UItem>(this, Item);
		InventoryItems.Add(NewItem);

		// Item has unlimited stacks and is stackable
		if(NewItem->IsStackable() && NewItem->HasUnlimitedStacks())
		{
			// Add the leftover stacks and break from our loop
			NewItem->AddStacks(Amount);
			Amount = 0;
			break;
		}
		else if(NewItem->IsStackable())
		{
			const int32 AvailableStacks = GetAvailableStackCount(NewItem);

			if((Amount - AvailableStacks) <= 0 || NewItem->HasUnlimitedStacks())
			{
				NewItem->AddStacks(Amount);
				Amount = 0;
				break;
			}

			NewItem->AddStacks(AvailableStacks);
			Amount -= AvailableStacks;
		}
		else
		{
			NewItem->AddStacks(1);
			Amount -= 1;
		}
	}
	
	return bAddedItems;
	
}

void UInventorySystemComponent::InitInventorySystemComponent()
{
	// Add Default Inventory Items to our Inventory
	for(TSubclassOf<UItem> Item : DefaultInventoryItems)
	{
		AddItem(Item, 1);
	}
}

TArray<UItem*> UInventorySystemComponent::GetInventoryItems()
{
	return InventoryItems;
}

bool UInventorySystemComponent::ContainsValidItemOfClass(const TSubclassOf<UItem>& Item, TArray<UItem*>& OutItemsFound)
{
	for(UItem* InventoryItem : InventoryItems)
	{
		// Check if are InventoryItem is of the same class / is a stackable item / its current stacks are less than th max stacks allowed or it has unlimited stacks
		if(InventoryItem->IsA(Item) && InventoryItem->IsStackable() && (GetAvailableStackCount(InventoryItem) > 0 || InventoryItem->HasUnlimitedStacks()))
		{
			OutItemsFound.Add(InventoryItem);
		}
	}

	// If our items found is greater than 0 return true
	return OutItemsFound.Num() > 0 ? true : false;
}

void UInventorySystemComponent::AddStacksToItems(TArray<UItem*>& Items, int32& StacksToGrant)
{
	// Iterate through our Items of the same type until we have no stacks to give or run out of items to update 
	for(UItem* Item : Items)
	{
		const int32 AvailableStacks = GetAvailableStackCount(Item);

		if((StacksToGrant - AvailableStacks) <= 0 || Item->HasUnlimitedStacks())
		{
			Item->AddStacks(StacksToGrant);
			StacksToGrant = 0;
			return;
		}

		Item->AddStacks(AvailableStacks);
		StacksToGrant -= AvailableStacks;
	}
}

int32 UInventorySystemComponent::GetAvailableStackCount(UItem* Item)
{
	return Item->GetMaxItemStacks() - Item->GetCurrentItemStacks();
}

bool UInventorySystemComponent::HasUnlimitedInventorySize() const
{
	return MaxInventorySize == -1;
}

bool UInventorySystemComponent::IsInventoryFull() const
{
	return InventoryItems.Num() >= MaxInventorySize || HasUnlimitedInventorySize();
}
