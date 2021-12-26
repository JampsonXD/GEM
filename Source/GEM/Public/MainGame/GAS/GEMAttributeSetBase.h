// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GEMAttributeSetBase.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class GEM_API UGEMAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

	public:

	UGEMAttributeSetBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGEMAttributeSetBase, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGEMAttributeSetBase, MaxHealth)

	// Damage Meta Attribute, not replicated. Will only be used on Server
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UGEMAttributeSetBase, Damage)

	/** Non Combat Attributes **/
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_MovementSpeed)
    FGameplayAttributeData MovementSpeed;
    ATTRIBUTE_ACCESSORS(UGEMAttributeSetBase, MovementSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Character Level", ReplicatedUsing = OnRep_CharacterLevel)
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(UGEMAttributeSetBase, CharacterLevel)

	UPROPERTY(BlueprintReadOnly, Category = "Character Experience", ReplicatedUsing = OnRep_Experience)
	FGameplayAttributeData Experience;
	ATTRIBUTE_ACCESSORS(UGEMAttributeSetBase, Experience)

	UPROPERTY(BlueprintReadOnly, Category = "Coins", ReplicatedUsing = OnRep_Coins)
	FGameplayAttributeData Coins;
	ATTRIBUTE_ACCESSORS(UGEMAttributeSetBase, Coins)
	

	float MinMovementSpeed;
	float MaxMovementSpeed;

	protected:

	// Hit React Tags
	FGameplayTag HitReactFront;
	FGameplayTag HitReactBack;
	FGameplayTag HitReactRight;
	FGameplayTag HitReactLeft;
	FGameplayTag Headshot;
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	protected:
	
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	
	UFUNCTION()
	virtual void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed);

	UFUNCTION()
	virtual void OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel);

	UFUNCTION()
	virtual void OnRep_Coins(const FGameplayAttributeData& OldCoins);

	UFUNCTION()
	virtual void OnRep_Experience(const FGameplayAttributeData& OldExperience);
};
