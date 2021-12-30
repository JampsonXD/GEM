// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Components/ActorComponent.h"
#include "InventorySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UInventorySystemComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

protected:
	
	// Our Array of Items the component currently has
	UPROPERTY(BlueprintReadOnly, Category = "Inventory System Component | Items")
	TArray<UItem*> InventoryItems;

	/** Maximum size of our components inventory **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory System Component")
	int32 MaxInventorySize;
	
	/* Array of Item Classes to grant our Component when initialized
	adding one or more of the same class will add a new item or update the existing
	stack count of an item based on its stackable setting
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory System Component | Defaults")
	TArray<TSubclassOf<UItem>> DefaultInventoryItems;

public:

	/** Tries adding an item of class to our inventory
	 * @param Item : The item class to add
	 *  @param Amount : The amount of the item we want to try to add
	 *  @return : Returns true if we successfully added an item
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Items")
	bool AddItem(TSubclassOf<UItem> Item, int32 Amount = 1);

	/** Initializes our Default Inventory Items
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory System Component | Defaults")
	void InitInventorySystemComponent();

	/** Getter for our Inventory Items Array
	 * @return TArray<UItem*> : Returns our Inventory Items
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UItem*> GetInventoryItems();

protected:

	UFUNCTION()
	bool ContainsValidItemOfClass(const TSubclassOf<UItem>& Item, TArray<UItem*>& OutItemsFound);

	/** Adds available stacks to items as long as we still have stacks to grant
	 * @param Items : Array of items we are trying to add stacks to
	 * @param StacksToGrant : The amount of stacks to try to add
	 * @return : Returns false if we are unable to add stacks to the item
	 */
	UFUNCTION()
	static void AddStacksToItems(TArray<UItem*>& Items, int32& StacksToGrant);

	/** Returns the available stack count - Max Stacks - Current Stacks
	 * @param Item : Item to get available stacks from 
	 */
	UFUNCTION()
	static int32 GetAvailableStackCount(UItem* Item);
	
	UFUNCTION()
	bool HasUnlimitedInventorySize() const;
	
	bool IsInventoryFull() const;
};