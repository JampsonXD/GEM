// Copyright Epic Games, Inc. All Rights Reserved.

#include "MainGame/GEMGameMode.h"
#include "GameFramework/SpectatorPawn.h"
#include "MainGame/GEMCharacter.h"
#include "MainGame/GEMPlayerController.h"

AGEMGameMode::AGEMGameMode()
{
	RespawnTimer = 5.f;
}

// Start Respawning the character, move the player over to a Spectator Pawn and start the respawn countdown
void AGEMGameMode::StartRespawn(AController* Controller)
{
	if(Controller->IsPlayerController())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride =ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ASpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(SpectatorClass, Controller->GetPawn()->GetActorTransform(), SpawnParams);

		Controller->UnPossess();
		Controller->Possess(SpectatorPawn);

		FTimerHandle RespawnTimerHandle;
		FTimerDelegate RespawnDelegate;

		RespawnDelegate = FTimerDelegate::CreateUObject(this, &AGEMGameMode::RespawnCharacter, Controller);
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, RespawnTimer, false);

		// If our Controller is a GEMPlayerController, Start our Respawn Timer Widget
		AGEMPlayerController* PC = Cast<AGEMPlayerController>(Controller);
		if(PC)
		{
			PC->SetRespawnCountdown(RespawnTimer);
		}
	}
}

// Respawn the Character, currently just uses the Default Pawn Class to Respawn
void AGEMGameMode::RespawnCharacter(AController* Controller)
{
	// Player Respawn Logic
	if(Controller->IsPlayerController())
	{
		AActor* PlayerStart = FindPlayerStart(Controller);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride =ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FTransform RespawnTransform;
		RespawnTransform = PlayerStart->GetActorTransform();
		RespawnTransform.SetScale3D(FVector(1.f,1.f,1.f));

		// See if our controller is a GEMPlayerController, if we are we want to override the Respawn Transform with our own SpawnTransform
		// Call our Respawn Widget Stop Function
		AGEMPlayerController* PlayerController = Cast<AGEMPlayerController>(Controller);
		if(PlayerController)
		{
			RespawnTransform = PlayerController->GetSpawnTransform();
			PlayerController->StopRespawnCountdown();
		}

		AGEMCharacter* Character = GetWorld()->SpawnActor<AGEMCharacter>(DefaultPawnClass, RespawnTransform, SpawnParams);

		ASpectatorPawn* SpectatorPawn = Cast<ASpectatorPawn>(Controller->GetPawn());
		Controller->UnPossess();
		SpectatorPawn->Destroy();
		Controller->Possess(Character);
	}
}
