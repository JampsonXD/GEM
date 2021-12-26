// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GEMTypes.h"
#include "Engine/GameInstance.h"
#include "GEMGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class GEM_API UGEMGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Game")
	FString SaveGameSlot;

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Game Session")
	//void JoinGameSession(FBlueprintSessionResult SessionResult, APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Save Game")
	void SavePlayerData(FPlayerData PlayerData);
	
	
};
