// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GEMTypes.h"
#include "GEMGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GEM_API UGEMGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	// Getter for InputID
	EAbilityInput GetInputID() const;

	// Overriding Check Cost function so we can also check our blueprint GEMCheckCost function, for things unrelated to GE costs
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;

	// Our own Check Cost function for things such as ammo checking on our weapons
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability")
	bool GEMCheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo) const;

	// Overriding Apply Cost so we can also apply the cost from our blueprint GEMApplyCost function, for things unrelated to GE costs
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	// Our own Apply Cost function for things such as ammo costs from our weapons
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability")
	void GEMApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const;
	
	// Expose GetSourceObject to Blueprint. Retrieves the SourceObject associated with this ability. Callable on non instanced abilities.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability", meta = (DisplayName = "Get Source Object"))
	UObject* K2_GetSourceObject(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo) const;
	

protected:
	// Use this to set which input to assign this ability to
	UPROPERTY(EditDefaultsOnly, Category = "Ability Defaults")
	EAbilityInput InputID;

	// Whether we should activate this ability as soon as it is granted to the ASC
	UPROPERTY(EditDefaultsOnly, Category = "Ability Defaults")
	bool bActivatedOnGranted;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
