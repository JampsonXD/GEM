// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGame/UI/UW_ExperiencePopup.h"

void UUW_ExperiencePopup::SetExperienceAmount(float InExpAmount) const
{
	FFormatNamedArguments Args;
	Args.Add(PopupExpKey, FText::AsNumber(InExpAmount));
	Exp_Text->SetText(FText::Format(FTextFormat(PopupText), Args));
}
