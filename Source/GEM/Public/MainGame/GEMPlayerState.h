// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GEMAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "GAS/GEMAttributeSetBase.h"
#include "GAS/GEMAttributeSetAmmo.h"
#include "GEMPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthChangedSignature, const float, NewHealthValue);

/**
 * 
 */
UCLASS()
class GEM_API AGEMPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Our Ability System Component **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System Component", meta = (AllowPrivateAccess = "true"))
	class UGEMAbilitySystemComponent* AbilitySystemComponent;

	/** Our Core Attribute Set **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute Set", meta = (AllowPrivateAccess = "true"))
	class UGEMAttributeSetBase* AttributeSet;

	/** Our Ammo Weapon Set **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute Set", meta = (AllowPrivateAccess = "true"))
	class UGEMAttributeSetAmmo* AmmoAttributeSet;

	public:

	// Constructor
	AGEMPlayerState();

	virtual void BeginPlay() override;

	// Attribute Getters
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaximumHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetCoinAmount() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMovementSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetCharacterLevel() const;

	// Attribute Changed Callback Functions
	void OnHealthChanged(const FOnAttributeChangeData& Data);

	// Experience Changed Callback Function
	void OnExperienceChanged(const FOnAttributeChangeData& Data);

	// Tag Changed Callback Functions
	virtual void CombatTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintAssignable)
	FHealthChangedSignature OnPlayerHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Death")
	bool IsAlive() const;

	public:
	
	
	/** Returns AbilitySystemComponent subobject **/
	FORCEINLINE class UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }
	/** Returns Core Attribute Set subobject **/
	FORCEINLINE class UGEMAttributeSetBase* GetAttributeSet() const { return AttributeSet; }
	/** Returns Ammo Attribute Set subobject **/
	FORCEINLINE class UGEMAttributeSetAmmo* GetAmmoAttributeSet() const { return AmmoAttributeSet; }
	
};
