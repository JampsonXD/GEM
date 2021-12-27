// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGame/ActorComponents/QuestSystemComponent.h"


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
	return true;
}

bool UQuestSystemComponent::SwapActiveQuest(UQuest* NewActiveQuest)
{
	return true;
}
