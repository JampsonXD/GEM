// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UW_RespawnTimer.generated.h"

/**
 * 
 */
UCLASS()
class GEM_API UUW_RespawnTimer : public UUserWidget
{
	GENERATED_BODY()

	public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* RespawnProgressBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* RespawnText;

	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void InitializeRespawnTimer(float InRespawnTime);
	
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void UpdateRespawnTimer(float InRespawnTime);

	UFUNCTION(BlueprintImplementableEvent, Category = "Respawn")
	void StartRespawnTimer();

	UFUNCTION(BlueprintImplementableEvent, Category = "Respawn")
	void StopRespawnTimer();

	protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "Respawn")
	float TotalRespawnCooldown;

	UPROPERTY(BlueprintReadWrite, Category = "Respawn")
	float CurrentRespawnCooldown;
	
};
