// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystemComponent.h"


// Sets default values for this component's properties
UInventorySystemComponent::UInventorySystemComponent(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Initialize as Unlimited Inventory Size
	MaxInventorySize = -1;
	
	PrimaryComponentTick.bCanEverTick = true;
	
}


bool UInventorySystemComponent::AddItem(TSubclassOf<UItem> Item, int32 StacksToGrant)
{
	// Make sure we are actually adding an amount or have a valid Item Class
	if(StacksToGrant <= 0 || !IsValid(Item))
	{
		return false;
	}
	
	TArray<UItem*> ValidItems;
	bool bAddedItems = false;

	// Check if we are a stackable item and already have items of the same type
	if(Cast<UItem>(Item->GetDefaultObject())->IsStackable() && ContainsValidItemOfClass(Item,ValidItems))
	{
		bAddedItems = true;
		AddStacksToItems(ValidItems, StacksToGrant);
	}
	
	// We have no stacks to update left or we have no inventory space
	if(StacksToGrant <= 0 || IsInventoryFull())
	{
		return bAddedItems;
	}

	// We have space and are going to at least create one instance of UItem so change
	// our boolean to true
	bAddedItems = true;
	
	// Continue creating item instances from class until we have no stacks to give left or our inventory is full
	while(!IsInventoryFull() && StacksToGrant > 0)
	{
		// Create an Item Instance and Re-Evaluate if we have stacks leftover
		CreateItemInstance(Item, StacksToGrant);
	}
	
	return bAddedItems;
}

bool UInventorySystemComponent::CreateItemInstance(TSubclassOf<UItem> Item, int32& StacksToGrant)
{
	// Create a new Item Instance
	UItem* NewItem = NewObject<UItem>(this, Item);
	InventoryItems.Add(NewItem);

	// Broadcast that we have a new item added to our Inventory
	if(OnItemAdded.IsBound())
	{
		OnItemAdded.Broadcast(NewItem);
	}

	// Only add one stack and return if our item is not stackable
	if(!NewItem->IsStackable())
	{
		NewItem->AddStacks(1);
		StacksToGrant -= 1;
		return false;
	}

	// Return true if we added all the available stacks to the item, false otherwise
	return AddAvailableStacksToItem(StacksToGrant, NewItem) ? true : false;
}

void UInventorySystemComponent::InitInventorySystemComponent()
{
	// Add Default Inventory Items to our Inventory
	for(const TSubclassOf<UItem> Item : DefaultInventoryItems)
	{
		AddItem(Item, 1);
	}
}

TArray<UItem*> UInventorySystemComponent::GetInventoryItems()
{
	return InventoryItems;
}

FOnItemAdded& UInventorySystemComponent::GetOnItemAddedDelegate()
{
	return OnItemAdded;
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
		// Returns true if we have no stacks leftover
		if (AddAvailableStacksToItem(StacksToGrant, Item)) return;
	}
}

bool UInventorySystemComponent::AddAvailableStacksToItem(int32& StacksToGrant, UItem* Item)
{
	const int32 AvailableStacks = GetAvailableStackCount(Item);

	if(StacksToGrant - AvailableStacks <= 0 || Item->HasUnlimitedStacks())
	{
		Item->AddStacks(StacksToGrant);
		StacksToGrant = 0;
		return true;
	}

	Item->AddStacks(AvailableStacks);
	StacksToGrant -= AvailableStacks;
	return false;
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
	return InventoryItems.Num() >= MaxInventorySize && !HasUnlimitedInventorySize();
}
