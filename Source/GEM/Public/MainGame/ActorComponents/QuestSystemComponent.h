// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MainGame/Questing/Quest.h"
#include "QuestSystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GEM_API UQuestSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UQuestSystemComponent();

	// Getters
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest System Component")
	TArray<UQuest*> GetQuests() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Quest System Component")
	UQuest* GetActiveQuest() const;

	/** Tries to add a new Quest to the set of quests this component will look over
	* @param NewQuest : Quest to try to add to our component
	* @param bSetAsActiveQuest : Whether we should set our new Quest as the Active Quest
	* @return bool : Returns whether the Quest was added successfully or not
	*/
	UFUNCTION(BlueprintCallable, Category = "Quest System Component")
	bool AddQuest(UQuest* NewQuest, bool bSetAsActiveQuest);

	/** Swaps the Active Quest with a new Quest
	 * @param NewActiveQuest : Quest we are trying to swap
	 * @return  bool : Whether the swap was successful or not
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest System Component")
	bool SwapActiveQuest(UQuest* NewActiveQuest);
	
protected:

	// Current list of quests this component owns
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quests")
	TArray<UQuest*> Quests;

	// The Quest this component is actively tracking
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quests")
	UQuest* ActiveQuest;
 
};
