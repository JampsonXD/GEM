// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.h"
#include "UObject/Object.h"
#include "Item.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentStackCountChanged, int32, OldStackCount, int32, NewStackCount);

/**
 * 
 */


UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYSTEM_API UItem : public UObject
{
	GENERATED_UCLASS_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Item | Info")
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, Category = "Item | Info")
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, Category = "Item | Info")
	class UTexture2D* ItemImage;

	// Whether this item can be stacked or not, defaults to false
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	bool bIsStackable;

	// If we can be stacked, our max stack count, defaults to -1 for if it is not stackable
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	int32 MaxStackCount;

	// Current stack count of our item
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	int32 CurrentStackCount;

	// Delegate for when our Current Stack Count Changes
	UPROPERTY(BlueprintAssignable)
	FOnCurrentStackCountChanged OnCurrentStackCountChanged;
	
	
public:

	/* Functionality called when user tries to interact with the item */
	UFUNCTION(BlueprintCallable, Category = "Item | Interaction")
	virtual void Use();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Item")
	bool IsStackable();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Item")
	bool HasUnlimitedStacks();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Item")
	int32 GetMaxItemStacks();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Item")
	int32 GetCurrentItemStacks();

	// Makes a struct of data about the item we can use in things like our UI
	UFUNCTION(BlueprintPure , BlueprintCallable, Category = "Item Data")
	FItemUIData MakeItemData();

	UFUNCTION()
	FOnCurrentStackCountChanged& GetOnCurrentStackCountChangedDelegate();

	/** Adds Stacks to our current item
	 * @param StacksToAdd : Amount of stacks we will be adding to our UItem
	*/
	UFUNCTION()
	void AddStacks(int32 StacksToAdd);
	
};
