// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MainGame/Questing/Quest.h"
#include "QuestSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNewActiveQuest, UQuest*, NewActiveQuest, UQuest*, OldActiveQuest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewQuestAdded, UQuest*, NewQuest);

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

	/** Quest Delegates Getters **/
	UFUNCTION()
	FOnNewActiveQuest GetOnNewActiveQuestDelegate() const;
	
protected:

	// Current list of quests this component owns and are not finished
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quests")
	TArray<UQuest*> CurrentQuests;

	// List of Quests our component has already finished
	UPROPERTY(BlueprintReadOnly, Category = "Quests | Finished")
	TArray<UQuest*> FinishedQuests;

	// The Quest this component is actively tracking
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quests")
	UQuest* ActiveQuest;

	/** Quest Delegates **/

	// A new Active Quest was set
	UPROPERTY(BlueprintAssignable)
	FOnNewActiveQuest NewActiveQuestDelegate;

	// A new Quest was added to our Quest Array
	UPROPERTY(BlueprintAssignable)
	FOnNewQuestAdded NewQuestAddedDelegate;

	/** Function that we will call whenever we want to change our Active Quest, calls delegates
	*@param NewQuest : Quest we are setting as our new Active Quest
	*/
	UFUNCTION()
	void SetActiveQuest(UQuest* NewQuest);

	/** Checks to see if we already have this quest added to our Quests Array
	 *@param InQuest : Quest we will be checking with against our Quest Array
	 *@return bool : Returns true if we already have the quest
	 */
	UFUNCTION()
	bool ContainsQuest(UQuest* InQuest);
 
};
