// Fill out your copyright notice in the Description page of Project Settings.

#include "ASyncTasks/AsyncTaskAttributeChanged.h"


UAsyncTaskAttributeChanged* UAsyncTaskAttributeChanged::ListenForAttributeChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute)
{
	/** Single Attribute Version **/
	/** Create a new ASync Task, verify our attribute and ASC is valid and bind AttributeChanged to gameplayattribute change delegate**/
	UAsyncTaskAttributeChanged* WaitForAttributeChangedTask = NewObject< UAsyncTaskAttributeChanged>();
	WaitForAttributeChangedTask->ASC = AbilitySystemComponent;
	WaitForAttributeChangedTask->AttributeToListenFor = Attribute;

	if (!AbilitySystemComponent || !Attribute.IsValid())
	{
		WaitForAttributeChangedTask->RemoveFromRoot();
		return nullptr;
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(WaitForAttributeChangedTask, &UAsyncTaskAttributeChanged::AttributeChanged);

	return WaitForAttributeChangedTask;
}

UAsyncTaskAttributeChanged* UAsyncTaskAttributeChanged::ListenForAttributesChange(UAbilitySystemComponent* AbilitySystemComponent, TArray<FGameplayAttribute> Attributes)
{
	/** Multiple Attribute Version **/
	/** Create a new ASync Task, verify our attributes and ASC is valid and bind AttributeChanged to gameplayattribute change delegate**/
	UAsyncTaskAttributeChanged* WaitForAttributesChangedTask = NewObject< UAsyncTaskAttributeChanged>();
	WaitForAttributesChangedTask->ASC = AbilitySystemComponent;
	WaitForAttributesChangedTask->AttributesToListenFor = Attributes;

	if (!AbilitySystemComponent || Attributes.Num() < 1)
	{
		WaitForAttributesChangedTask->RemoveFromRoot();
		return nullptr;
	}

	for (FGameplayAttribute Attribute : Attributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(WaitForAttributesChangedTask, &UAsyncTaskAttributeChanged::AttributeChanged);
	}

	return WaitForAttributesChangedTask;
}

void UAsyncTaskAttributeChanged::EndTask()
{
	/** Remove any bindings and mark the object for destroyed **/
	if (IsValid(ASC))
	{
		ASC->GetGameplayAttributeValueChangeDelegate(AttributeToListenFor).RemoveAll(this);

		for (FGameplayAttribute Attribute : AttributesToListenFor)
		{
			ASC->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this);
		}
	}

	SetReadyToDestroy();
	MarkPendingKill();
}

void UAsyncTaskAttributeChanged::AttributeChanged(const FOnAttributeChangeData& Data)
{
	// Make sure our values actually changed and they arent getting clamped somewhere to the same value
	if (Data.OldValue == Data.NewValue)
	{
		return;
	}
	// OnAttributeChanged is used for BPP assignable functionality, the other is used for CPP implementations
	OnAttributeChanged.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);
}
