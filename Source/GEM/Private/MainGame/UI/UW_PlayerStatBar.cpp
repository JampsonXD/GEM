// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGame/UI/UW_PlayerStatBar.h"

void UUW_PlayerStatBar::SetupInitialHealth(UAbilitySystemComponent* ASC)
{
	if(!ASC)
	{
		SetHealthText(0.f,0.f);
		SetHealthBarPercent(0.f);
	}
	
	CurrentHealth =  ASC->GetNumericAttribute(UGEMAttributeSetBase::GetHealthAttribute());
	MaxHealth = ASC->GetNumericAttribute(UGEMAttributeSetBase::GetMaxHealthAttribute());
	
	SetHealthText(CurrentHealth, MaxHealth);
	SetHealthBarPercent(CurrentHealth / MaxHealth);
}

void UUW_PlayerStatBar::SetCurrentHealth(float InNewCurrentHealth)
{
	CurrentHealth = InNewCurrentHealth;
	UpdateHealthUI();
}

void UUW_PlayerStatBar::SetMaximumHealth(float InNewMaxHealth)
{
	MaxHealth = InNewMaxHealth;
	UpdateHealthUI();
}

void UUW_PlayerStatBar::SetHealthText(float InCurrentHealth, float InMaxHealth) const
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("CurrentHealth"), FText::AsNumber(InCurrentHealth));
	Args.Add(TEXT("MaxHealth"), FText::AsNumber(InMaxHealth));
	HealthText->SetText(FText::Format(FTextFormat(HealthTextFormat), FFormatNamedArguments(Args)));
}

void UUW_PlayerStatBar::SetHealthBarPercent(float InPercent) const
{
	HealthProgressBar->SetPercent(InPercent);
}

void UUW_PlayerStatBar::UpdateHealthUI() const
{
	SetHealthText(CurrentHealth, MaxHealth);
	SetHealthBarPercent(CurrentHealth / MaxHealth);
}
