// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GEM_Singleton.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GEM_API UGEM_Singleton : public UObject
{
	GENERATED_BODY()
	
public:
	
	UGEM_Singleton();

private:

	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> InCombatEffect;
	
};