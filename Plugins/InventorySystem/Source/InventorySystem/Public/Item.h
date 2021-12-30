// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Item.generated.h"

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

	UFUNCTION()
	void AddStacks(int32 Amount);
	
};
