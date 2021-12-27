// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGame/ActorComponents/QuestSystemComponent.h"

#include "GEM/GEM.h"


// Sets default values for this component's properties
UQuestSystemComponent::UQuestSystemComponent()
{
	// Can't tick
	PrimaryComponentTick.bCanEverTick = false;
}

TArray<UQuest*> UQuestSystemComponent::GetQuests() const
{
	return Quests;
}

UQuest* UQuestSystemComponent::GetActiveQuest() const
{
	return ActiveQuest;
}

bool UQuestSystemComponent::AddQuest(UQuest* NewQuest, bool bSetAsActiveQuest)
{
	if(ContainsQuest(NewQuest))
	{
		GEM_PRINTSCREEN("Quest already contained in our Quest Array!")
		return false;
	}

	// Add the Quest to our Quests Array
	Quests.Add(NewQuest);

	// Delegate Call
	if(NewQuestAddedDelegate.IsBound())
	{
		NewQuestAddedDelegate.Broadcast(NewQuest);
	}

	// SetAsActiveQuest is true or we just added our only Quest
	if(bSetAsActiveQuest || Quests.Num() == 1)
	{
		SetActiveQuest(NewQuest);
	}

	return true;
}

bool UQuestSystemComponent::SwapActiveQuest(UQuest* NewActiveQuest)
{
	// Current Quest is already New Active Quest
	if(NewActiveQuest == ActiveQuest)
	{
		return false;
	}

	SetActiveQuest(NewActiveQuest);
	return true;
	
}

FOnNewActiveQuest UQuestSystemComponent::GetOnNewActiveQuestDelegate() const
{
	return NewActiveQuestDelegate;
}

void UQuestSystemComponent::SetActiveQuest(UQuest* NewQuest)
{
	UQuest* OldQuest = ActiveQuest;
	ActiveQuest = NewQuest;
	if(NewActiveQuestDelegate.IsBound())
	{
		NewActiveQuestDelegate.Broadcast(ActiveQuest, OldQuest);
	}
}

// Quests use Gameplay tags as their Identifiers, check if this quest is already owned by checking if tags match
bool UQuestSystemComponent::ContainsQuest(UQuest* InQuest)
{
	for(UQuest* Quest : Quests)
	{
		if(Quest->GetQuestID().MatchesTag(InQuest->GetQuestID()))
		{
			return true;
		}
	}

	return false;
}
