// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GEMItemData.generated.h"

/**
 * 
 */
UCLASS()
class GEM_API UGEMItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item);
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	UTexture2D* ItemArt;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	int32 ItemLevel;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
