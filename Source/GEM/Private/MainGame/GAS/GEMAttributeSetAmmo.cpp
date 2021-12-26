// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGame/GAS/GEMAttributeSetAmmo.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UGEMAttributeSetAmmo::UGEMAttributeSetAmmo() : Super()
{
}

void UGEMAttributeSetAmmo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGEMAttributeSetAmmo, RifleAmmo, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGEMAttributeSetAmmo, PistolAmmo, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGEMAttributeSetAmmo, ShotgunAmmo, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGEMAttributeSetAmmo, SniperAmmo, COND_OwnerOnly, REPNOTIFY_Always);
}

void UGEMAttributeSetAmmo::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Ammo cannot go under 0
	if(Data.EvaluatedData.Attribute == GetRifleAmmoAttribute())
	{
		SetRifleAmmo(FMath::Max(GetRifleAmmo(), 0.f));
	}

	if(Data.EvaluatedData.Attribute == GetPistolAmmoAttribute())
	{
		SetPistolAmmo(FMath::Max(GetPistolAmmo(), 0.f));
	}

	if(Data.EvaluatedData.Attribute == GetShotgunAmmoAttribute())
	{
		SetShotgunAmmo(FMath::Max(GetShotgunAmmo(), 0.f));
	}

	if(Data.EvaluatedData.Attribute == GetSniperAmmoAttribute())
	{
		SetSniperAmmo(FMath::Max(GetSniperAmmo(), 0.f));
	}
}

void UGEMAttributeSetAmmo::OnRep_RifleAmmo(const FGameplayAttributeData& OldRifleAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGEMAttributeSetAmmo, RifleAmmo, OldRifleAmmo);
}

void UGEMAttributeSetAmmo::OnRep_PistolAmmo(const FGameplayAttributeData& OldPistolAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGEMAttributeSetAmmo, PistolAmmo, OldPistolAmmo);
}

void UGEMAttributeSetAmmo::OnRep_ShotgunAmmo(const FGameplayAttributeData& OldShotgunAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGEMAttributeSetAmmo, ShotgunAmmo, OldShotgunAmmo);
}

void UGEMAttributeSetAmmo::OnRep_SniperAmmo(const FGameplayAttributeData& OldSniperAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGEMAttributeSetAmmo, SniperAmmo, OldSniperAmmo);
}
