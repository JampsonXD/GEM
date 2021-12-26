// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGame/GAS/ECC_SuperDamage.h"
#include "AbilitySystemComponent.h"
#include "GEM/GEM.h"

struct AttributeStruct
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	AttributeStruct()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGEMAttributeSetBase, Damage, Source, true);
	}
};

static const AttributeStruct& AttributeCapture()
{
	static AttributeStruct Attributes;
	return Attributes;
}


// Constructor
UECC_SuperDamage::UECC_SuperDamage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Initialize Variables
	SourceASC = nullptr;
	TargetASC = nullptr;
	SourceActor = nullptr;
	TargetActor = nullptr;

	// Add Relevant Attributes to Capture
	RelevantAttributesToCapture.Add(AttributeCapture().DamageDef);
}


void UECC_SuperDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	float Damage = 0.f;
	// Capture damage value set on the GE
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttributeCapture().DamageDef, EvaluationParameters, Damage);
	// Any set by caller damage if found
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Value1")), false, -1.f), 0.f);

	if(Damage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(AttributeCapture().DamageProperty, EGameplayModOp::Additive, Damage));
	}
	
}
