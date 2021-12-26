// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGame/GAS/GEMGameplayAbility.h"
#include "AbilitySystemComponent.h"

EAbilityInput UGEMGameplayAbility::GetInputID() const
{
	return InputID;
}

/** Overridden Check Cost and Apply Cost functions from UGameplayAbility **/
bool UGEMGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags) && GEMCheckCost(Handle, *ActorInfo);
}

void UGEMGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo) const
{
	GEMApplyCost(Handle, *ActorInfo, ActivationInfo);
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);
}

/** Blueprint Callable Function to get our Source Object **/
UObject* UGEMGameplayAbility::K2_GetSourceObject(FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo& ActorInfo) const
{
	return GetSourceObject(Handle, &ActorInfo);
}

// Overwrite in Blueprints to do additional cost checks
bool UGEMGameplayAbility::GEMCheckCost_Implementation(FGameplayAbilitySpecHandle,
                                                      const FGameplayAbilityActorInfo& ActorInfo) const
{
	return true;
}

// Overwrite in Blueprints for additional application of costs
void UGEMGameplayAbility::GEMApplyCost_Implementation(FGameplayAbilitySpecHandle,
                                                      const FGameplayAbilityActorInfo& ActorInfo, FGameplayAbilityActivationInfo ActivationInfo) const
{
	
}

// Check if we are wanting to activate the effect as soon as we grant it, for things such as passives
void UGEMGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if(bActivatedOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}
