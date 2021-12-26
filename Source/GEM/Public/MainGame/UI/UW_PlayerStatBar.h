// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MainGame/GAS/GEMAttributeSetBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "UW_PlayerStatBar.generated.h"

/**
 * 
 */
UCLASS()
class GEM_API UUW_PlayerStatBar : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "HUD | Health")
	void SetupInitialHealth(UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "HUD | Health")
	void SetCurrentHealth(float InNewCurrentHealth);

	UFUNCTION(BlueprintCallable, Category = "HUD | Health")
	void SetMaximumHealth(float InNewMaxHealth);
	
protected:

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UVerticalBox* VerticalBoxContainer;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* HealthText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UProgressBar* HealthProgressBar;
	
	UFUNCTION()
	void SetHealthText(float InCurrentHealth, float InMaximumHealth) const;

	UFUNCTION()
	void SetHealthBarPercent(float InPercent) const;

	UFUNCTION()
	void UpdateHealthUI() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD | Health")
	FText HealthTextFormat;

	UPROPERTY()
	float CurrentHealth = 0.f;

	UPROPERTY()
	float MaxHealth = 0.f;
};
