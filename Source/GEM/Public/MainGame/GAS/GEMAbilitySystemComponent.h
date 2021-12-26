// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GEMAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class GEM_API UGEMAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	public:

	bool CharacterAbilitiesGiven = false;
	bool StartupEffectsApplied = false;
};
