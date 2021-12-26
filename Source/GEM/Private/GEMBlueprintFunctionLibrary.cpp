// Fill out your copyright notice in the Description page of Project Settings.


#include "GEMBlueprintFunctionLibrary.h"

#include "AbilitySystemInterface.h"
#include "GEMLoadingScreen.h"
#include "Kismet/GameplayStatics.h"

void UGEMBlueprintFunctionLibrary::PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime)
{
	IGEMLoadingScreenModule& LoadingScreen = IGEMLoadingScreenModule::Get();
	LoadingScreen.StartInGameLoadingScreen(bPlayUntilStopped, PlayTime);
}

void UGEMBlueprintFunctionLibrary::StopLoadingScreen()
{
	IGEMLoadingScreenModule& LoadingScreen = IGEMLoadingScreenModule::Get();
	LoadingScreen.StopInGameLoadingScreen();
}

bool UGEMBlueprintFunctionLibrary::ArrayIsEmpty(TArray<UObject*> InArray)
{
	return InArray.Num() == 0;
}

bool UGEMBlueprintFunctionLibrary::IsLocalRole(const AActor* InActor)
{
	const ENetRole LocalRole = InActor->GetLocalRole();
	const ENetMode NetMode = InActor->GetNetMode();

	return LocalRole == ROLE_AutonomousProxy || (LocalRole == ROLE_Authority && NetMode != NM_DedicatedServer);
}

bool UGEMBlueprintFunctionLibrary::IsControllerLocal(const AController* InController)
{
	// Validate Controller isn't null
	if(!InController)
	{
		return false;
	}

	return InController->IsLocalController();
}

UGEMGameInstance* UGEMBlueprintFunctionLibrary::GetGEMGameInstance(const UObject * WorldContextObject)
{
	UGEMGameInstance* GameInstanceRef = Cast<UGEMGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject->GetWorld()));
	return GameInstanceRef ? GameInstanceRef : nullptr;
}

UGEMAbilitySystemComponent* UGEMBlueprintFunctionLibrary::GetGEMAbilitySystemComponentFromActor(AActor* InActor)
{
	// Check we implement IAbilitySystemInterface
	IAbilitySystemInterface* Interface = Cast<IAbilitySystemInterface>(InActor);
	if(Interface)
	{
		// Cast our Interface call to GEMASC
		UGEMAbilitySystemComponent* GEMAbilitySystemComponent = Cast<UGEMAbilitySystemComponent>(Interface->GetAbilitySystemComponent());
		if(GEMAbilitySystemComponent)
		{
			return GEMAbilitySystemComponent;
		}
	}

	// Checks failed, return a null pointer
	return nullptr;
}

void UGEMBlueprintFunctionLibrary::CancelAllAbilities(UAbilitySystemComponent* ASC)
{
	if(!ASC)
	{
		return;
	}

	ASC->CancelAllAbilities();
}
