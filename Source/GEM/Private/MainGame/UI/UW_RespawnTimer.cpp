// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGame/UI/UW_RespawnTimer.h"

void UUW_RespawnTimer::InitializeRespawnTimer(float InRespawnTime)
{
	TotalRespawnCooldown = InRespawnTime;
	CurrentRespawnCooldown = InRespawnTime;
}

void UUW_RespawnTimer::UpdateRespawnTimer(float InRespawnTime)
{
	// If our respawn is set to 0 or below we destroy the widget
	if(InRespawnTime <= 0.f)
	{
		Destruct();
	}

	// Set new respawn timer
	CurrentRespawnCooldown = InRespawnTime;
}
