// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UW_ExperiencePopup.generated.h"

/**
 * 
 */
UCLASS()
class GEM_API UUW_ExperiencePopup : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void SetExperienceAmount(float InExpAmount) const;

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD | Exp")
	void ShowExperience();

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Exp_Text;

	UPROPERTY(EditDefaultsOnly, Category = "HUD | Exp")
	FText PopupText;

	UPROPERTY(EditDefaultsOnly, Category = "HUD | Exp")
	FString PopupExpKey;
};
