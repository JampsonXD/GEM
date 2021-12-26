// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGame/UI/UW_DeathRecap.h"

#include "MainGame/DataAssets/GEMCharacterData.h"

void UUW_DeathRecap::PlayDeathRecap(const UGEMCharacterData* InEnemyCharacterData)
{
	// Set our formatted text
	FFormatNamedArguments Args;
	Args.Add(TEXT("PlayerName"), FText::FromName(InEnemyCharacterData->ItemName));
	DeathRecap_Text->SetText( FText::Format(FTextFormat(DeathRecapPopupText),Args));

	// Set our enemy image
	DeathRecapEnemy_Image->SetBrushFromTexture(InEnemyCharacterData->ItemArt);

	// Play our animation
	PlayAnimation(DeathRecap_Animation);
	
}
