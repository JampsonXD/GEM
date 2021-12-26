// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEMAttributeSetBase.h"
#include "MainGame/GEMCharacter.h"
#include "ECC_SuperDamage.generated.h"


/**
 * 
 */
UCLASS()
class GEM_API UECC_SuperDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_UCLASS_BODY()

protected:

	/** Execute Variables **/
	mutable UAbilitySystemComponent* SourceASC;
	mutable UAbilitySystemComponent* TargetASC;
	mutable AActor* SourceActor;
	mutable AActor* TargetActor;
	mutable FAggregatorEvaluateParameters EvaluationParameters;
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	
};
