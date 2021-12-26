// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UW_DeathRecap.generated.h"

class UGEMCharacterData;
/**
 * 
 */
UCLASS()
class GEM_API UUW_DeathRecap : public UUserWidget
{
	GENERATED_BODY()


public:

	UFUNCTION()
	void PlayDeathRecap(const UGEMCharacterData* InEnemyCharacterData);

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* HorizontalBoxContainer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* DeathRecap_Text;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* DeathRecapEnemy_Image;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* DeathRecap_Animation = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "HUD | Death Recap")
	FText DeathRecapPopupText;
};
