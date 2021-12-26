// Fill out your copyright notice in the Description page of Project Settings.


#include "ASyncTasks/AT_WaitForNextTick.h"

UAT_WaitForNextTick::UAT_WaitForNextTick(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

UAT_WaitForNextTick* UAT_WaitForNextTick::WaitForNextTick(UGameplayAbility* OwningAbility)
{
	UAT_WaitForNextTick* WaitForNextTick = NewAbilityTask<UAT_WaitForNextTick>(OwningAbility);
	return WaitForNextTick;
}

void UAT_WaitForNextTick::Activate()
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UAT_WaitForNextTick::BroadcastNextTick);
}

void UAT_WaitForNextTick::BroadcastNextTick()
{
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		OnNextTick.Broadcast();
	}
	EndTask();
}
