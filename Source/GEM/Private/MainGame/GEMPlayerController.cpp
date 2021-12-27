// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGame/GEMPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GEM/GEM.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainGame/GEMCharacter.h"
#include "MainGame/GEMPlayerState.h"
#include "MainGame/GEMWeapon.h"
#include "Net/UnrealNetwork.h"

AGEMPlayerController::AGEMPlayerController()
{
	QuestSystemComponent = CreateDefaultSubobject<UQuestSystemComponent>(TEXT("Quest System Component"));
	SpawnTransform = FTransform(FVector(0.f,0.f,0.f));
}

UQuestSystemComponent* AGEMPlayerController::GetQuestSystemComponent() const
{
	return QuestSystemComponent;
}

void AGEMPlayerController::CreateHUD()
{
	// Dont run this if we already have a created HUD
	if(HUD)
	{
		return;
	}

	if(!IsValid(HUDClass))
	{
		GEM_PRINTSCREEN(HUDClass->GetName() + " is not valid! Please select a proper HUD default!")
	}

	// If we are not playing a standalone game and are not a local player controller
	if(!UKismetSystemLibrary::IsStandalone(this))
	{
		// Make sure we are a local player controller
		if(!IsLocalPlayerController())
		{
			return;
		}
	}

	// Create our Widget, Add to Viewport
	HUD = CreateWidget<UUW_HUD>(this, HUDClass);
	HUD->AddToViewport();
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);


	// Check if our Player Character is ready and set its Weapon Values if so, if not we can set later when we init the weapon
	AGEMCharacter* PlayerCharacter = GetPawn<AGEMCharacter>();
	if(PlayerCharacter)
	{
		// Setup Weapon Data for HUD
		AGEMWeapon* Weapon =  PlayerCharacter->GetCurrentWeapon();
		if(Weapon)
		{
			SetHUDWeaponData(Weapon);
		}
	}
}

UUW_HUD* AGEMPlayerController::GetGEMHUD()
{
	return HUD;
}

void AGEMPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	// Create our HUD - TODO : Need to find a better way of Creating our HUD once PlayerState and Player is Replicated down. Currently HUD Construct runs a timed loop waiting for replication
	CreateHUD();
}

void AGEMPlayerController::SetHUDWeaponData(AGEMWeapon* InWeapon)
{
	SetHUDWeaponVisibility(IsValid(InWeapon));
	
	if(!InWeapon)
	{
		return;
	}
	
	HUD->SetWeaponDataInfo(InWeapon->GetWeaponData());
	SetCurrentAmmoText(InWeapon->GetCurrentAmmo());
	SetMaximumAmmoText(InWeapon->GetMaximumAmmo());
}

void AGEMPlayerController::SetSpawnTransform_Implementation(FTransform Transform)
{
	// Keep our scale as 1
	Transform.SetScale3D(FVector(1.f,1.f,1.f));
	SpawnTransform = Transform;
}

FTransform AGEMPlayerController::GetSpawnTransform() const
{
	return SpawnTransform;
}

void AGEMPlayerController::SetRespawnCountdown_Implementation(float TimeRemaining)
{
	// Make sure our widget is valid, if not, Create the widget using our Widget Class
	if(!RespawnWidget)
	{
		RespawnWidget = Cast<UUW_RespawnTimer>(CreateWidget(this, RespawnWidgetClass));
		RespawnWidget->AddToViewport(0);
	}

	// Call to Hide certain HUD functionality while respawning
	if(HUD)
	{
		HUD->StartRespawn();
	}
	
	// Initialize our values and start our respawn timer, Start Respawn Timer will also show the widget on the screen
	RespawnWidget->InitializeRespawnTimer(TimeRemaining);
	RespawnWidget->StartRespawnTimer();
		
}

bool AGEMPlayerController::SetRespawnCountdown_Validate(float TimeRemaining)
{
	return true;
}

void AGEMPlayerController::StopRespawnCountdown_Implementation()
{
	if(!RespawnWidget)
	{
		return;
	}

	// Functionality for our HUD to show specific widgets and things on respawn
	if(HUD)
	{
		HUD->EndRespawn();
	}
	
	RespawnWidget->StopRespawnTimer();
	
}

bool AGEMPlayerController::StopRespawnCountdown_Validate()
{
	return true;
}

void AGEMPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGEMPlayerController, SpawnTransform);
}

void AGEMPlayerController::SetWeaponInfoFromWeaponData(UGEMWeaponData* InWeaponData)
{
	HUD->SetWeaponDataInfo(InWeaponData);
}

void AGEMPlayerController::SetAmmoInfoFromAmmoData(UGEMAmmoData* InAmmoData)
{
	HUD->SetAmmoDataInfo(InAmmoData);
}

void AGEMPlayerController::SetHUDWeaponVisibility(bool bShow)
{
	bShow ? HUD->ToggleWeaponInfoVisibility(ESlateVisibility::HitTestInvisible) : HUD->ToggleWeaponInfoVisibility(ESlateVisibility::Hidden);
}

void AGEMPlayerController::SetHUDWeaponName(FName InWeaponName)
{
	HUD->SetWeaponName(InWeaponName);
}

void AGEMPlayerController::SetCurrentAmmoText(int InCurrentAmmo)
{
	HUD->SetWeaponCurrentAmmoText(InCurrentAmmo);
}

void AGEMPlayerController::SetMaximumAmmoText(int InMaximumAmmo)
{
	HUD->SetWeaponMaximumAmmoText(InMaximumAmmo);
}
