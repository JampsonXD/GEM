// Copyright Epic Games, Inc. All Rights Reserved.

#include "MainGame/GEMCharacter.h"
#include <string>
#include "GEMTypes.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GEM/GEM.h"
#include "MainGame/GEMCharacterMovementComponent.h"
#include "MainGame/GEMGameMode.h"
#include "MainGame/GEMPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GEMAssetManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainGame/GEMPlayerController.h"
#include "MainGame/GEMWeapon.h"

//////////////////////////////////////////////////////////////////////////
// AGEMCharacter

AGEMCharacter::AGEMCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UGEMCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->bUsePawnControlRotation = true;

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("First Person Mesh"));
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FirstPersonMesh->SetCollisionProfileName(FName("No Collision"));
	FirstPersonMesh->bReceivesDecals = false;
	FirstPersonMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	FirstPersonMesh->CastShadow = false;
	FirstPersonMesh->SetVisibility(false, true);
	// Owning Player should only see our first person mesh
	FirstPersonMesh->bOnlyOwnerSee = true;

	// Our third person mesh, owning player should not see this, only other players
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->SetupAttachment(RootComponent);
	

	// Animations
	CurrentStance = EStanceType::OutOfCombat;

	HitReactFront = FGameplayTag::RequestGameplayTag("Effect.HitReact.Front");
	HitReactBack = FGameplayTag::RequestGameplayTag("Effect.HitReact.Back");
	HitReactRight = FGameplayTag::RequestGameplayTag("Effect.HitReact.Right");
	HitReactLeft = FGameplayTag::RequestGameplayTag("Effect.HitReact.Left");
	Headshot = FGameplayTag::RequestGameplayTag("Effect.DamagePopup.Headshot");

	// Weapon Tags
	NoWeaponTag = FGameplayTag::RequestGameplayTag("Weapons.None");
	CurrentWeaponTag = NoWeaponTag;

	// Damage Popup
	DamagePopupDelayRate = 0.1f;
}

USkeletalMeshComponent* AGEMCharacter::GetFirstPersonMesh()
{
	return FirstPersonMesh;
}

FPrimaryAssetId AGEMCharacter::GetCharacterInfo() const
{
	return CharaterInfo;
}

UAbilitySystemComponent* AGEMCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGEMCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Enhanced Input key bindings
	if(UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		PlayerEnhancedInputComponent->BindAction(MovementInputAction.InputAction, ETriggerEvent::Triggered, this, &AGEMCharacter::MoveCharacter);
		PlayerEnhancedInputComponent->BindAction(RotateInputAction.InputAction, ETriggerEvent::Triggered, this, &AGEMCharacter::RotateCharacter);
		PlayerEnhancedInputComponent->BindAction(JumpInputAction.InputAction, ETriggerEvent::Started, this, &AGEMCharacter::Jump);
		PlayerEnhancedInputComponent->BindAction(JumpInputAction.InputAction, ETriggerEvent::Completed, this, &AGEMCharacter::StopJumping);
	}
	// Bind Ability Input, also ran in OnRep_PlayerState because of race condition
	BindASCInput();
}

void AGEMCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGEMCharacter, CurrentStance);
	DOREPLIFETIME(AGEMCharacter, CurrentWeapon);
	DOREPLIFETIME_CONDITION(AGEMCharacter, CharacterInventory, COND_OwnerOnly);
}

void AGEMCharacter::InitCharacterStats()
{
	// Verify our ASC is valid and our Base Stats are valid
	if(!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if(!DefaultAttributes)
	{
		GEM_PRINTSCREEN("No Default Stats Setup for Character")
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
	if(Spec.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), AbilitySystemComponent.Get());
	}
	

	
}

void AGEMCharacter::GrantDefaultAbilities()
{
	// Make sure we are called on the server and that our ASC is valid and that we haven't granted the starting abilities already
	if(GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	if(DefaultAbilities.Num() == 0)
	{
		GEM_PRINTSCREEN("No Default Abilities Found")
		return;
	}

	for(TSubclassOf<UGEMGameplayAbility>& Ability : DefaultAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->GetInputID()), this));
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;
	
}

void AGEMCharacter::BindASCInput()
{
	if(!ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("Confirm"), FString("Cancel"), FString("EAbilityInput"), static_cast<int32>(EAbilityInput::Confirm), static_cast<int32>(EAbilityInput::Cancel)));

		ASCInputBound =  true;
	}
}

void AGEMCharacter::OnDefaultWeaponLoaded(FPrimaryAssetId IDLoaded)
{
	UAssetManager* AssetManager = UAssetManager::GetIfValid();
	if(AssetManager)
	{
		UGEMWeaponData* WeaponData = Cast<UGEMWeaponData>(AssetManager->GetPrimaryAssetObject(IDLoaded));

		if(WeaponData)
		{
			AddWeaponToInventory(FInventoryWeaponData(WeaponData), true);
		}
	}
}

AGEMWeapon* AGEMCharacter::GetCurrentWeapon()
{
	return CurrentWeapon;
}

bool AGEMCharacter::AddWeaponToInventory(FInventoryWeaponData NewWeaponData, bool bEquipWeapon)
{
	if(GetLocalRole() < ROLE_Authority)
	{
		return false;
	}
	
	if(bEquipWeapon)
	{
		// Spawn our weapon and use its WeaponData to populate the data of the weapon
		AGEMWeapon* PreviousWeapon = CurrentWeapon;
		AGEMWeapon* NewWeapon =  SpawnWeaponDeferred(NewWeaponData);
		if(NewWeapon)
		{
			SetCurrentWeapon(NewWeapon, PreviousWeapon);
			// Set our WeaponPointer to our new Weapon
			NewWeaponData.WeaponPointer = NewWeapon;
		}
		
	}

	CharacterInventory.Weapons.Add(NewWeaponData);

	return true;
}

FName AGEMCharacter::GetWeaponAttachPoint()
{
	return WeaponAttachPoint;
}

bool AGEMCharacter::FindAndUpdateWeaponAmmo(FInventoryWeaponData& InInventoryWeapon)
{

	// Validate our WeaponData passed in actually contains a Weapon Pointer
	if(!InInventoryWeapon.WeaponPointer)
	{
		return false;
	}
	
	for(FInventoryWeaponData& WeaponObject : CharacterInventory.Weapons)
	{
		if(WeaponObject.WeaponPointer == InInventoryWeapon.WeaponPointer)
		{
			// We found a match, update the values ammo count to our passed in WeaponData structs ammo
			WeaponObject.Ammo = InInventoryWeapon.Ammo;
			return true;
		}
	}
	return false;
}

void AGEMCharacter::CurrentWeaponAmmoChanged(int32 OldAmmoCount, int32 NewAmmoCount)
{
	AGEMPlayerController* PC = Cast<AGEMPlayerController>(GetController());
	if(PC && PC->IsLocalPlayerController())
	{
		PC->SetCurrentAmmoText(NewAmmoCount);
	}
}

void AGEMCharacter::MaximumWeaponAmmoChanged(int32 OldAmmoCount, int32 NewAmmoCount)
{
	AGEMPlayerController* PC = Cast<AGEMPlayerController>(GetController());
	if(PC && PC->IsLocalPlayerController())
	{
		PC->SetMaximumAmmoText(NewAmmoCount);
	}
}

void AGEMCharacter::SetCurrentWeapon(AGEMWeapon* NewWeapon, AGEMWeapon* OldWeapon)
{
	if(NewWeapon == OldWeapon)
	{
		return;
	}

	if(OldWeapon)
	{
		// Remove our Weapon Tag if our ASC is still valid - Run on all clients because Loose tags are not replicated
		if(AbilitySystemComponent.IsValid())
		{
			AbilitySystemComponent->RemoveLooseGameplayTag(CurrentWeaponTag);
			CurrentWeaponTag = NoWeaponTag;
			AbilitySystemComponent->AddLooseGameplayTag(CurrentWeaponTag);
		}
		
		UnequipWeapon(OldWeapon);
	}
	
	if(NewWeapon)
	{
		if(AbilitySystemComponent.IsValid())
		{
			AbilitySystemComponent->RemoveLooseGameplayTag(CurrentWeaponTag);
		}
		CurrentWeapon = NewWeapon;
		CurrentWeapon->SetOwningCharacter(this);
		CurrentWeapon->EquipWeapon();
		// Sets things such as our Skeletal Mesh when replicatated
		CurrentWeapon->SetValuesFromWeaponData();
		// Add our Weapon Abilities - Validates that only run on Authority in function
		CurrentWeapon->AddAbilities();

		// Add our Weapon Tag to our gameplay tags
		CurrentWeaponTag = CurrentWeapon->GetWeaponTag();
		if(AbilitySystemComponent.IsValid())
		{
			AbilitySystemComponent->AddLooseGameplayTag(CurrentWeaponTag);
		}

		
		AGEMPlayerController* PC = Cast<AGEMPlayerController>(GetController());
		if(PC && PC->IsLocalPlayerController())
		{
			// Sets visibility based on whether our weapon is valid or not
			PC->SetHUDWeaponVisibility(IsValid(CurrentWeapon));

			// Set our Weapon Information
			PC->SetWeaponInfoFromWeaponData(CurrentWeapon->GetWeaponData());
			PC->SetAmmoInfoFromAmmoData(CurrentWeapon->GetAmmoData());
			PC->SetCurrentAmmoText(CurrentWeapon->GetCurrentAmmo());
			PC->SetMaximumAmmoText(CurrentWeapon->GetMaximumAmmo());
		}

		// Bind to Weapon Ammo Change Delegates
		CurrentWeapon->CurrentAmmoChanged.AddDynamic(this, &AGEMCharacter::CurrentWeaponAmmoChanged);
		CurrentWeapon->MaxAmmoChanged.AddDynamic(this, &AGEMCharacter::MaximumWeaponAmmoChanged);
	}
	
	// If we make it this far, we should be either removing or adding a weapon either way.
	// See if anything is bound to our weapon changed delegate and send the new weapon, can be nullptr;
	if(OnWeaponChanged.IsBound())
	{
		OnWeaponChanged.Broadcast(NewWeapon);
	}
}

void AGEMCharacter::UnequipWeapon(AGEMWeapon* Weapon)
{
	// Create a WeaponData to add back to our inventory using data found on our weapon
	UGEMWeaponData* WeaponDataObject = Weapon->GetWeaponData();
	int32 CurrentAmmoCount = Weapon->GetCurrentAmmo();
	
	FInventoryWeaponData InventoryObject = FInventoryWeaponData(*WeaponDataObject, CurrentAmmoCount);
	InventoryObject.WeaponPointer = Weapon;
	
	// Search through our array and update the value in here
	FindAndUpdateWeaponAmmo(InventoryObject);
	
	// Remove any abilities we had with this Weapon - Validates that this is only run on server in function
	CurrentWeapon->RemoveAbilities();

	// Unbind to our Weapon Delegates
	CurrentWeapon->CurrentAmmoChanged.RemoveDynamic(this, &AGEMCharacter::CurrentWeaponAmmoChanged);
	CurrentWeapon->MaxAmmoChanged.RemoveDynamic(this, &AGEMCharacter::MaximumWeaponAmmoChanged);
	
	// Unequip Weapon and Destroy Actor
	CurrentWeapon->UnequipWeapon();
}

void AGEMCharacter::OnRep_CurrentWeapon(AGEMWeapon* PreviousWeapon)
{
	SetCurrentWeapon(CurrentWeapon, PreviousWeapon);
}

AGEMWeapon* AGEMCharacter::SpawnWeaponDeferred(FInventoryWeaponData WeaponData)
{
	// Get World Object and Create a Transform
	UWorld* World = GetWorld();
	const FTransform SpawnLocationAndRotation = GetTransform();

	// Break open our WeaponData Struct
	const UGEMWeaponData* WeaponDataAsset = WeaponData.WeaponData;

	// Spawn our actor deferred so we can setup our Data Asset variables
	AGEMWeapon* Weapon = World->SpawnActorDeferred<AGEMWeapon>(WeaponDataAsset->WeaponClass,SpawnLocationAndRotation);
	Weapon->SetupData(WeaponData);
	Weapon->FinishSpawning(SpawnLocationAndRotation);

	// Set this as our new current weapon
	return Weapon;
}

bool AGEMCharacter::EquipNextWeapon()
{
	// Validate that we have atleast one item in the array to loop through
	if(CharacterInventory.Weapons.Num() <= 0)
	{
		GEM_PRINTSCREEN("No Weapons to Equip")
		return false;
	}

	
	int32 CurrentIndex = -1;
	// Find our current index point if our weapon is valid
	if(CurrentWeapon)
	{
		FindCurrentWeaponIndex(CurrentIndex);
	}

	// If our index is invalid, set it to the first in the array
	if(CurrentIndex == -1)
	{
		CurrentIndex = 0;
	}

	// New Index set to our current Index, makes sure we dont equip our weapon if its already equipped
	int32 NewIndex = CurrentIndex;

	// Loop through our weapon array up to the size of the inventory
	int32 i = 0;
	while(i != CharacterInventory.Weapons.Num())
	{
		// Increment New Index
		NewIndex += 1;

		// Make sure our index is inside of the inventory space
		if(NewIndex == CharacterInventory.Weapons.Num())
		{
			NewIndex = 0;
		}
		// We found a valid index, return true and equip this item
		if(CharacterInventory.Weapons.IsValidIndex(NewIndex) && NewIndex != CurrentIndex)
		{
			EquipWeaponAtIndex(NewIndex);
			return true;
		}

		// Increment Iterator
		i++;
	}
	return false;
}

bool AGEMCharacter::EquipLastWeapon()
{
	// Validate that we have atleast one item in the array to loop through
	if(CharacterInventory.Weapons.Num() <= 0)
	{
		GEM_PRINTSCREEN("No Weapons to Equip")
		return false;
	}

	
	int32 CurrentIndex = -1;
	// Find our current index point if our weapon is valid
	if(CurrentWeapon)
	{
		FindCurrentWeaponIndex(CurrentIndex);
	}

	// If our index is the first in the array or invalid, set it to the last
	if(CurrentIndex == 0 || CurrentIndex == -1)
	{
		CurrentIndex = CharacterInventory.Weapons.Num() - 1;
	}

	// New Index set to our current Index, makes sure we dont equip our weapon if its already equipped
	int32 NewIndex = CurrentIndex;
	
	// Loop through our weapon array up to the size of the inventory
	int32 i = 0;
	while(i != CharacterInventory.Weapons.Num())
	{
		// Decrement New Index
		NewIndex -= 1;

		// Make sure our index is not a negative value
		if(NewIndex == -1)
		{
			NewIndex = CharacterInventory.Weapons.Num() - 1;
		}
		// We found a valid index, return true and equip this item
		if(CharacterInventory.Weapons.IsValidIndex(NewIndex) && NewIndex != CurrentIndex)
		{
			EquipWeaponAtIndex(NewIndex);
			return true;
		}

		// Increment iterator
		i++;
	}
	return false;
}

void AGEMCharacter::EquipWeaponAtIndex(int32 WeaponIndex)
{
	if(GetLocalRole() < ROLE_Authority)
	{
		return;
	}

	// Spawn our weapon and use its WeaponData to populate the data of the weapon
	AGEMWeapon* PreviousWeapon = CurrentWeapon;
	AGEMWeapon* NewWeapon =  SpawnWeaponDeferred(CharacterInventory.Weapons[WeaponIndex]);
	if(NewWeapon)
	{
		SetCurrentWeapon(NewWeapon, PreviousWeapon);
		CharacterInventory.Weapons[WeaponIndex].WeaponPointer = NewWeapon;
	}
}

bool AGEMCharacter::FindCurrentWeaponIndex(int32& InIndex)
{
	for(int32 i = 0; i != CharacterInventory.Weapons.Num(); ++i)
	{
		// Our Current Weapon is equal to our Weapon Pointer, return true and our index
		if (CharacterInventory.Weapons[i].WeaponPointer == CurrentWeapon)
		{
			InIndex = i;
			return true;
		}
	}
	// We did not find the weapon, return false
	return false;
}

void AGEMCharacter::MoveCharacter(const FInputActionValue& ActionValue)
{
	// Validate our player controller
	if (!Controller)
	{
		return;
	}
	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector MoveForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// get right vector
	const FVector MoveRightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Get our action mapping scales; X = 0, Y = 1; X and Y both are negated for MoveLeft and MoveBackwards inside of our Input Mapping Context
	const float MoveForwardScale = ActionValue[1];
	const float MoveRightScale = ActionValue[0];
	
	AddMovementInput(MoveForwardDirection, MoveForwardScale, false);
	AddMovementInput(MoveRightDirection, MoveRightScale, false);

	// GEM_PRINTSCREEN(ActionValue.ToString())
}

// Rotate our Character
void AGEMCharacter::RotateCharacter(const FInputActionValue& ActionValue)
{
	const float LookUpRate = ActionValue[1];
	const float TurnRightRate = ActionValue[0];

	AddControllerYawInput(TurnRightRate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	AddControllerPitchInput(LookUpRate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	
}

EHitReactDirection AGEMCharacter::GetHitReactDirection(const FVector& ImpactPoint) const
{
	const FVector ActorLocation = GetActorLocation();
	// Point plane distance to get how far our impact point is from our actor
	const float DistanceToFrontBackPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorRightVector());
	const float DistanceToRightLeftPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorForwardVector());

	// If our FrontBack Distance is closer, return our Front or Back animation based on the floats value; Else return our Right or Left animation based on its float value
	if(FMath::Abs(DistanceToFrontBackPlane) <= FMath::Abs(DistanceToRightLeftPlane))
	{
		return DistanceToFrontBackPlane >= 0 ? EHitReactDirection::Front : EHitReactDirection::Back;
	}
	else
	{
		return DistanceToRightLeftPlane >= 0 ? EHitReactDirection::Right : EHitReactDirection::Left;
	}
}

void AGEMCharacter::PlayHitReact_Implementation(FGameplayTag HitDirection)
{
	// Verify we are still alive
	if(IsAlive())
	{
		if(HitDirection == HitReactFront)
		{
			ShowHitReact.Broadcast(EHitReactDirection::Front);
		}
		else if(HitDirection == HitReactBack)
		{
			ShowHitReact.Broadcast(EHitReactDirection::Back);
		}
		else if(HitDirection == HitReactRight)
		{
			ShowHitReact.Broadcast(EHitReactDirection::Right);
		}
		else if(HitDirection == HitReactLeft)
		{
			ShowHitReact.Broadcast(EHitReactDirection::Left);
		}
	}
}

bool AGEMCharacter::PlayHitReact_Validate(FGameplayTag HitDirection)
{
	return true;
}

void AGEMCharacter::Execute_Death_Implementation()
{
	if(AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAllAbilities();
		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}

	// Destroy our weapon if we die. If we don't call this, our weapon actor will end up just stuck where our character died
	if(CurrentWeapon)
	{
		SetCurrentWeapon(nullptr, CurrentWeapon);
	}

	// Play Death Montage, anim notify will be located inside that will call our Finish Death function
	if(DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
		return;
	}

	FinishDeath();
}

void AGEMCharacter::AddDamagePopupToQueue_Implementation(FDamagePopupData PopupData, FGameplayTagContainer BonusEffectTags)
{
	// Add our newest damage value to the queue
	PopupData.Tags.AppendTags(BonusEffectTags);
	DamagePopupQueue.Add(PopupData);
	
	// Start our Damage Popup Timer if its not already valid
	if(!GetWorldTimerManager().IsTimerActive(DamagePopupTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(DamagePopupTimerHandle, this, &AGEMCharacter::ShowDamagePopup, 0.1f, true, 0.0f);
	}
}

void AGEMCharacter::ShowDamagePopup()
{
	// Add a new Damage Popup Component to our player if we are valid and we have another damage number to show
	if(DamagePopupQueue.Num() > 0 && IsValid(this))
	{
		UGEMPopupDamageWC* DamagePopup = NewObject<UGEMPopupDamageWC>(this, DamagePopupClass);
		DamagePopup->RegisterComponent();
		DamagePopup->SetWorldLocation(DamagePopupQueue[0].Location);
		DamagePopup->SetupDamagePopup(DamagePopupQueue[0].DamageAmount, DamagePopupQueue[0].Tags);

		// Remove our  Damage we just used from the Queue
		DamagePopupQueue.RemoveAt(0);

		// Check and see if we have any more damage popup values, and invalidate the timer if not
		if(DamagePopupQueue.Num() < 1)
		{
			GetWorldTimerManager().ClearTimer(DamagePopupTimerHandle);
		}
	}
}

void AGEMCharacter::FinishDeath()
{
	// Only run on the server
	if(GetLocalRole() == ROLE_Authority)
	{
		AGEMGameMode* GM = Cast<AGEMGameMode>(GetWorld()->GetAuthGameMode());
		if(GM)
		{
			GM->StartRespawn(GetController());
		}
	}

	Destroy();
}

bool AGEMCharacter::IsAlive()
{
	// Validate AttributeSet is valid, return if our health is greater than 0
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth() > 0.f;
	}

	// AttributeSet is not valid, assume we are dead
	return false;
}

float AGEMCharacter::GetMovementSpeed() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetMovementSpeed();
	}
	
	return 0.f;
}

float AGEMCharacter::GetCharacterLevel() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetCharacterLevel();
	}
	
	return 0.f;
}

float AGEMCharacter::GetHealth() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth();
	}
	
	return 0.f;
}

float AGEMCharacter::GetMaxHealth() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxHealth();
	}
	
	return 0.f;
}

float AGEMCharacter::GetRifleReserveAmmo() const
{
	if(AmmoAttributeSet.IsValid())
	{
		return AmmoAttributeSet->GetRifleAmmo();
	}

	return 0.f;
}

void AGEMCharacter::SetHealth(float InHealthValue)
{
	if(AttributeSet.IsValid())
	{
		AttributeSet->SetHealth(InHealthValue);
	}
}

EStanceType AGEMCharacter::GetCurrentStance()
{
	return CurrentStance;
}

void AGEMCharacter::SetCurrentStance(EStanceType InType)
{
	CurrentStance = InType;
}

// Run on Server Only
void AGEMCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AGEMPlayerState* PS = GetPlayerState<AGEMPlayerState>();
	if(PS)
	{
		// Sets the ASC on the server
		AbilitySystemComponent = Cast<UGEMAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Init ASC Actor Info on the server - Owner is Player State, Avatar is our Character
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		// Set the AttributeSet for attribute functions
		AttributeSet = PS->GetAttributeSet();

		// Set our Ammo AttributeSet
		AmmoAttributeSet = PS->GetAmmoAttributeSet();

		// Initialize Starting Attributes
		InitCharacterStats();

		// Remove the Death Tag from the character
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		// Init our Current Health as the Max Health Value set
		SetHealth(GetMaxHealth());

		// Grant our Default Abilities
		GrantDefaultAbilities();

		// Add our Current Weapon Tag
		AbilitySystemComponent->AddLooseGameplayTag(CurrentWeaponTag);

		// Standalone game to setup HUD
		AGEMPlayerController* PlayerController = Cast<AGEMPlayerController>(GetController());
		if(PlayerController && UKismetSystemLibrary::IsStandalone(this))
		{
			PlayerController->CreateHUD();
		}
		
		// If we have a default weapon, equip that weapon
		if(DefaultWeapon.IsValid())
		{
			// Get the Asset Manager
			UAssetManager* Manager = UAssetManager::GetIfValid();

			// Optional Bunldes
			const TArray<FName> Bundles;

			// Our delegate to call once we are loaded
			const FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &AGEMCharacter::OnDefaultWeaponLoaded, DefaultWeapon);
			Manager->LoadPrimaryAsset(DefaultWeapon, Bundles , Delegate);
		}
		else
		{
			SetCurrentWeapon(nullptr, CurrentWeapon);
		}
	}
}

void AGEMCharacter::FellOutOfWorld(const UDamageType& Dmg)
{
	FellOffWorld();
}

void AGEMCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	if(APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// Get our Enhanced Input Local Player Subsystem from our Local Player related to the owning Player Controller
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			// Clear any mappings, this can be run more than once in the pawns lifetime so clear any leftover mappings out
			Subsystem->ClearAllMappings();

			// Add Mapping Context, along with priority values; Higher values are prioritized
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGEMCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AGEMPlayerState* PS = GetPlayerState<AGEMPlayerState>();
	if(PS)
	{
		// Sets the ASC on the client
		AbilitySystemComponent = Cast<UGEMAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Init ASC Actor Info on the client - Owner is Player State, Avatar is our Character
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		// Bind ASC Input to Input Component, also ran in SetupPlayerInputComponent because of race condition
		BindASCInput();

		AttributeSet = PS->GetAttributeSet();

		AmmoAttributeSet = PS->GetAmmoAttributeSet();

		InitCharacterStats();

		// Set Current Health to Max
		SetHealth(GetMaxHealth());
		
		// Add our Current Weapon Tag
		AbilitySystemComponent->AddLooseGameplayTag(CurrentWeaponTag);
		
	}
}

void AGEMCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	// Clearing Weapon Tag on ASC, this is because we cleared the tag on the server during our death function, but was never replicated down to the client because they are marked pending destroy.
	if(AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(CurrentWeaponTag);
		CurrentWeaponTag = NoWeaponTag;
		AbilitySystemComponent->AddLooseGameplayTag(CurrentWeaponTag);
	}
	
	Super::EndPlay(EndPlayReason);
}
