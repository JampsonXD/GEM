// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGame/GEMCharacterMovementComponent.h"

#include "MainGame/GEMCharacter.h"

float UGEMCharacterMovementComponent::GetMaxSpeed() const
{
	AGEMCharacter* Owner = Cast<AGEMCharacter>(GetOwner());

	// If our cast is unsuccessful, return the default MaxSpeed set in the parent class
	if(!Owner)
	{
		return Super::GetMaxSpeed();
	}

	// Returning our Attribute Sets Movement Speed variable
	return Owner->GetMovementSpeed();
}
