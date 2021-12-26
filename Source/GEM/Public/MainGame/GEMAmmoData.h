// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "MainGame/GEMItemData.h"
#include "GEMAmmoData.generated.h"

/**
 * 
 */
UCLASS()
class GEM_API UGEMAmmoData : public UGEMItemData
{
	GENERATED_BODY()

public:
	
	UGEMAmmoData();

	// Gameplay Effect for our Ammo Consume
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	TSubclassOf<UGameplayEffect> AmmoConsumeClass;

	// What Attribute we are tied to, used in things such as our HUD to tell which attribute to listen for
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	FGameplayAttribute AmmoAttribute;
	
};
