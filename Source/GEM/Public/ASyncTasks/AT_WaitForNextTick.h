// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_WaitForNextTick.generated.h"

/** Multicast delegate used for Task **/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNextTick);

/**
 * 
 */
UCLASS()
class GEM_API UAT_WaitForNextTick : public UAbilityTask
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(BlueprintAssignable)
	FOnNextTick OnNextTick;
	
	/** Calls a Delegate Bound when the next tick occurs **/
	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAT_WaitForNextTick* WaitForNextTick(UGameplayAbility* OwningAbility);

	virtual void Activate() override;

private:
	
	UFUNCTION()
	void BroadcastNextTick();
	
};
