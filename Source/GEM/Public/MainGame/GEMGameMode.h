// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/GameModeBase.h"
#include "GEMGameMode.generated.h"

UCLASS(minimalapi)
class AGEMGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGEMGameMode();

	UFUNCTION()
	void StartRespawn(AController* Controller);

	UFUNCTION()
	void RespawnCharacter(AController* Controller);

	UPROPERTY()
	float RespawnTimer;
};



