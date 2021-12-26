// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GEMAttributeSetAmmo.generated.h"

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
class GEM_API UGEMAttributeSetAmmo : public UAttributeSet
{
	GENERATED_BODY()

public:

	UGEMAttributeSetAmmo();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_RifleAmmo)
	FGameplayAttributeData RifleAmmo;
	ATTRIBUTE_ACCESSORS(UGEMAttributeSetAmmo, RifleAmmo)
	
	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_PistolAmmo)
	FGameplayAttributeData PistolAmmo;
	ATTRIBUTE_ACCESSORS(UGEMAttributeSetAmmo, PistolAmmo)

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_ShotgunAmmo)
	FGameplayAttributeData ShotgunAmmo;
	ATTRIBUTE_ACCESSORS(UGEMAttributeSetAmmo, ShotgunAmmo)

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_SniperAmmo)
	FGameplayAttributeData SniperAmmo;
	ATTRIBUTE_ACCESSORS(UGEMAttributeSetAmmo, SniperAmmo)

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	

protected:
	
	UFUNCTION()
	virtual void OnRep_RifleAmmo(const FGameplayAttributeData& OldRifleAmmo);

	UFUNCTION()
	virtual void OnRep_PistolAmmo(const FGameplayAttributeData& OldPistolAmmo);

	UFUNCTION()
	virtual void OnRep_ShotgunAmmo(const FGameplayAttributeData& OldShotgunAmmo);

	UFUNCTION()
	virtual  void OnRep_SniperAmmo(const FGameplayAttributeData& OldSniperAmmo);
};
