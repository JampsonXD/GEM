// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GEMCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class GEM_API UGEMCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	public:

	virtual float GetMaxSpeed() const override;
	
};
