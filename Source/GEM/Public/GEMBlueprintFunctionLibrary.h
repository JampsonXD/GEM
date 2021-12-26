// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GEMGameInstance.h"
#include "MainGame/GAS/GEMAbilitySystemComponent.h"
#include "GEMBlueprintFunctionLibrary.generated.h"

class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class GEM_API UGEMBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Loading)
	static void PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime);

	UFUNCTION(BlueprintCallable, Category = Loading)
	static void StopLoadingScreen();

	// Checks whether an objects array is empty, returns true if so
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Array Helpers")
	static bool ArrayIsEmpty(TArray<UObject*> InArray);

	// Checks whether the actor is a local role, or the server but not in a dedicated game mode
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Effects")
	static bool IsLocalRole(const AActor* InActor);

	// Helper function that validates controller is valid and returns if the controller is locally controlled
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GEM Blueprint Function Library | Helper Functions")
	static bool IsControllerLocal(const AController* InController);

	// Get the GEM Game Instance
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GEM Game Instance",meta=(HidePin = "WorldContextObject", DefaultToSelf="WorldContextObject") )
	static UGEMGameInstance* GetGEMGameInstance(const UObject * WorldContextObject);

	// ============================================================================= //
	// ==============                   GAS                   ====================== //
	// ============================================================================= //

	// Get GEM Ability System Component from Actor
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GEM Blueprint Function Library | GAS")
	static UGEMAbilitySystemComponent* GetGEMAbilitySystemComponentFromActor(AActor* InActor);
	
	// Cancel all active abilities on the passed in ASC
	UFUNCTION(BlueprintCallable, Category = "GEM Blueprint Function Library | GAS")
	static void CancelAllAbilities(UAbilitySystemComponent* ASC);
	
};
