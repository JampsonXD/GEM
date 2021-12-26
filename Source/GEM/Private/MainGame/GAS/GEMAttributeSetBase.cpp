// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGame/GAS/GEMAttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "MainGame/GEMCharacter.h"
#include "MainGame/GEMPlayerController.h"
#include "Net/UnrealNetwork.h"

UGEMAttributeSetBase::UGEMAttributeSetBase() : Super()
{
	MinMovementSpeed = 200.0f;
	MaxMovementSpeed = 1500.0f;

	HitReactFront = FGameplayTag::RequestGameplayTag("Effect.HitReact.Front");
	HitReactBack = FGameplayTag::RequestGameplayTag("Effect.HitReact.Back");
	HitReactRight = FGameplayTag::RequestGameplayTag("Effect.HitReact.Right");
	HitReactLeft = FGameplayTag::RequestGameplayTag("Effect.HitReact.Left");
	Headshot = FGameplayTag::RequestGameplayTag("Effect.DamagePopup.Headshot");
}

void UGEMAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGEMAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGEMAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGEMAttributeSetBase, MovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGEMAttributeSetBase, CharacterLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGEMAttributeSetBase, Coins, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGEMAttributeSetBase, Experience, COND_None, REPNOTIFY_Always);
	
}

void UGEMAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetMovementSpeedAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, MinMovementSpeed, MaxMovementSpeed);
	}
}

void UGEMAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	AGEMCharacter* TargetCharacter = nullptr;
	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<AGEMCharacter>(TargetActor);
	}

	// Get the source actor
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	AGEMCharacter* SourceCharacter = nullptr;
	if(Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->PlayerController.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		
		if(SourceController == nullptr && SourceActor != nullptr)
		{
			if(APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		// Use the controller to find the source pawn
		if(SourceController)
		{
			SourceCharacter = Cast<AGEMCharacter>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<AGEMCharacter>(SourceActor);
		}

		if(Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}

	if(Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Grab our Damage and store it locally, reset the Damage Attribute back to 0
		float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if(LocalDamageDone > 0.f)
		{
			bool bHeadshot = false;
			bool WasAlive = true;
			if(TargetCharacter)
			{
				WasAlive = TargetCharacter->IsAlive();
			}

			// Check if we have a hit result, will use for Headshot damage and HitReact
			const FHitResult* HitResult = Data.EffectSpec.GetContext().GetHitResult();

			if(HitResult)
			{
				if(HitResult->BoneName == FName("head"))
				{
					bHeadshot = true;
					LocalDamageDone = LocalDamageDone * 2.0f;
				}
			}
			// Set our new Health amount based on Current Health and Damage Done
			const float NewHealth = GetHealth() - LocalDamageDone;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			if(WasAlive && TargetCharacter)
			{
				if(HitResult)
				{
					const EHitReactDirection HitReactDirection = TargetCharacter->GetHitReactDirection(HitResult->Location);
					switch (HitReactDirection)
					{
						case EHitReactDirection::Front:
							TargetCharacter->PlayHitReact(HitReactFront);
							break;
						case EHitReactDirection::Back:
							TargetCharacter->PlayHitReact(HitReactBack);
							break;
						case EHitReactDirection::Right:
							TargetCharacter->PlayHitReact(HitReactRight);
							break;
						case EHitReactDirection::Left:
							TargetCharacter->PlayHitReact(HitReactLeft);
							break;
						default: break;
					}
				}
				// Hit Result does not exist or couldn't be found, play default hit react in front
				else
				{
					TargetCharacter->PlayHitReact(HitReactFront);
				}

				// If our Source Character is also valid and is not our Target Character, add a damage popup to the source character
				if(SourceCharacter && SourceCharacter != TargetCharacter)
				{
					// Default to our Actor Location
					FVector PopupLocation = TargetCharacter->GetActorLocation();

					// If our hit result is valid, get the impact point to add the popup to
					if(HitResult)
					{
						PopupLocation = HitResult->ImpactPoint;
					}

					FDamagePopupData Popup = FDamagePopupData(PopupLocation, LocalDamageDone);
					// Check if we had a headshot and add our Headshot to the Tag Container to pass through
					FGameplayTagContainer DamagePopupTags;
					if(bHeadshot)
					{
						DamagePopupTags.AddTagFast(Headshot);
					}
					SourceCharacter->AddDamagePopupToQueue(Popup, DamagePopupTags);
				}
				// Someone died since we were alive before but arent now, extra functionality
				if(!TargetCharacter->IsAlive())
				{
					// UI/Additional Functionality called for Source and Target when a death occurs
					AGEMPlayerController* SourceGEMController = Cast<AGEMPlayerController>(SourceController);
					AGEMPlayerController* TargetGEMController = Cast<AGEMPlayerController>(TargetController);
					
					// Killed from someone other than ourselves
					if(SourceController != TargetController)
					{
						// We are a player controller and we want to do additional kill character functionality
						if(SourceGEMController)
						{
							SourceGEMController->KilledCharacter(TargetCharacter, TargetCharacter->GetCharacterLevel());
						}

						// Our target is a player controller and we want to have additional on death functionality
						if(TargetGEMController)
						{
							TargetGEMController->WasKilled(SourceCharacter, false);
						}
					}
					// Source is our Target as well and we are a valid player controller
					else if(TargetGEMController)
					{
						TargetGEMController->WasKilled(SourceCharacter, true);
					}
				}
			}
		}
	}
	
	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	
}

void UGEMAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGEMAttributeSetBase, Health, OldHealth);
}

void UGEMAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGEMAttributeSetBase, MaxHealth, OldMaxHealth);
}

void UGEMAttributeSetBase::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGEMAttributeSetBase, MovementSpeed, OldMovementSpeed);
}

void UGEMAttributeSetBase::OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGEMAttributeSetBase, CharacterLevel, OldCharacterLevel);
}

void UGEMAttributeSetBase::OnRep_Coins(const FGameplayAttributeData& OldCoins)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGEMAttributeSetBase, Coins, OldCoins);
}

void UGEMAttributeSetBase::OnRep_Experience(const FGameplayAttributeData& OldExperience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGEMAttributeSetBase, Experience, OldExperience);
}
