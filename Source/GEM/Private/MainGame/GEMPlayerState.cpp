// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGame/GEMPlayerState.h"

#include "MainGame/GEMCharacter.h"
#include "MainGame/GEMPlayerController.h"

AGEMPlayerState::AGEMPlayerState()
{
	//////////////////////////// Ability System Component ///////////////////////////////////////////////////////
	AbilitySystemComponent = CreateDefaultSubobject<UGEMAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Replication Mode for our ASC, only Attributes, GameplayTags, and Gameplaycues will be replicated to everyone
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Attribute Set
	AttributeSet = CreateDefaultSubobject<UGEMAttributeSetBase>(TEXT("Attribute Set"));

	// Ammo Attribute Set
	AmmoAttributeSet = CreateDefaultSubobject<UGEMAttributeSetAmmo>(TEXT("Ammo Attribute Set"));
	
	// Replication / Networking
	NetUpdateFrequency = 100;
}

void AGEMPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if(AbilitySystemComponent)
	{
		// Attribute Changed Listeners
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AGEMPlayerState::OnHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetExperienceAttribute()).AddUObject(this, &AGEMPlayerState::OnExperienceChanged);

		// Tag Changed Listeners
		AbilitySystemComponent->RegisterGameplayTagEvent(InCombatTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this,&AGEMPlayerState::CombatTagChanged);
	}
}

float AGEMPlayerState::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float AGEMPlayerState::GetMaximumHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float AGEMPlayerState::GetCoinAmount() const
{
	return AttributeSet->GetCoins();
}

float AGEMPlayerState::GetMovementSpeed() const
{
	return AttributeSet->GetMovementSpeed();
}

float AGEMPlayerState::GetCharacterLevel() const
{
	return AttributeSet->GetCharacterLevel();
}

void AGEMPlayerState::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	const float Health = Data.NewValue;
	AGEMCharacter* PlayerChar = Cast<AGEMCharacter>(GetPawn());

	// Broadcast for anything looking for this value update
	if(OnPlayerHealthChanged.IsBound())
	{
		OnPlayerHealthChanged.Broadcast(Health);
	}

	// Check if we are alive and dont already have the death tag, Kill the player if true
	if(!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		if(PlayerChar)
		{
			PlayerChar->Execute_Death();
		}
	}
}

void AGEMPlayerState::OnExperienceChanged(const FOnAttributeChangeData& Data)
{
	// Values didn't change or our new value isn't larger than our old value
	if(Data.NewValue <= Data.OldValue)
	{
		return;
	}

	// Player controller related to this PS
	AGEMPlayerController* PC = Cast<AGEMPlayerController>(GetOwner());
	if(!PC)
	{
		return;
	}

	// Add an ExperiencePopup to our screen if our HUD isn't null
	UUW_HUD* HUD = PC->GetGEMHUD();
	if(HUD)
	{
		HUD->AddExperiencePopup(Data.NewValue - Data.OldValue);
	}
}

void AGEMPlayerState::CombatTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	AGEMCharacter* PlayerChar = Cast<AGEMCharacter>(GetPawn());
	if(!PlayerChar)
	{
		return;
	}

	// Set our Combat Stance based on if we have the combat tag
	switch(NewCount)
	{
		case 0:
			PlayerChar->SetCurrentStance(EStanceType::OutOfCombat);
			break;
		case 1:
			PlayerChar->SetCurrentStance(EStanceType::MageCombat);
			break;
		default:
			break;
	}
}

bool AGEMPlayerState::IsAlive() const
{
	return GetHealth() > 0.f;
}
