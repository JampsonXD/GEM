// Fill out your copyright notice in the Description page of Project Settings.
#include "MainGame/GEMWeapon.h"

#include "GEMBlueprintFunctionLibrary.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GEM/GEM.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AGEMWeapon::AGEMWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;;

	// Set to replicate
	bReplicates = true;
	bNetUseOwnerRelevancy = true;

	// Default Weapon Stats
	CurrentAmmoCount = 0;
	MaxAmmoCount = 0;
	FireRate = 0;

	// Components
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(FName("Collision Component"));
	CollisionComponent->InitCapsuleSize(30.f, 40.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	WeaponMeshFirstPerson = CreateDefaultSubobject<USkeletalMeshComponent>(FName("First Person Skeletal Mesh"));
	WeaponMeshFirstPerson->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMeshFirstPerson->CastShadow = false;
	WeaponMeshFirstPerson->SetVisibility(false, true);
	WeaponMeshFirstPerson->SetupAttachment(CollisionComponent);
	WeaponMeshFirstPerson->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;

	HandguardMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Handguard"));
	HandguardMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandguardMesh->SetupAttachment(WeaponMeshFirstPerson, "handguard");

	StockMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Stock"));
	StockMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StockMesh->SetupAttachment(WeaponMeshFirstPerson, "stock");
	
	IronsightBackMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("BackIronSight"));
	IronsightBackMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	IronsightBackMesh->SetupAttachment(WeaponMeshFirstPerson, "rail_t");

	IronsightFrontMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("FrontIronSight"));
	IronsightFrontMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	IronsightFrontMesh->SetupAttachment(WeaponMeshFirstPerson, "rail_t");

	SightMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Sight Mesh"));
	SightMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SightMesh->SetupAttachment(WeaponMeshFirstPerson, "sight");

	ScopeSceneCapturer = CreateDefaultSubobject<USceneCaptureComponent2D>(FName("Scope Scene Capture"));
	ScopeSceneCapturer->bCaptureEveryFrame = false;
	ScopeSceneCapturer->bCaptureOnMovement = false;
	ScopeSceneCapturer->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDRNoAlpha;
	ScopeSceneCapturer->SetupAttachment(SightMesh);
	ScopeSceneCapturer->SetRelativeLocation(FVector(75.f, 0.f, 0.f));

	WeaponFiringTag = FGameplayTag::RequestGameplayTag("Weapons.Firing");
	
	// ADS Default Values
	PlayerADSFOV = 0.f;
	ScopeADSFOV = 0.f;
	ADSCameraOffset = 0.f;

	// Recoil Default Values
	RecoilHorizontal = 0.f;
	RecoilVertical = 0.f;
	
}

UAbilitySystemComponent* AGEMWeapon::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

USkeletalMeshComponent* AGEMWeapon::GetSkeletalMeshFirstPerson() const
{
	return WeaponMeshFirstPerson;
}

USkeletalMeshComponent* AGEMWeapon::GetStockMesh() const
{
	return StockMesh;
}

UStaticMeshComponent* AGEMWeapon::GetHandguardMesh() const
{
	return HandguardMesh;
}

UStaticMeshComponent* AGEMWeapon::GetIronsightFrontMesh() const
{
	return IronsightFrontMesh;
}

UStaticMeshComponent* AGEMWeapon::GetIronsightBackMesh() const
{
	return IronsightBackMesh;
}

USkeletalMeshComponent* AGEMWeapon::GetSight() const
{
	return SightMesh;
}

USceneCaptureComponent2D* AGEMWeapon::GetScopeSceneCapture() const
{
	return ScopeSceneCapturer;
}

void AGEMWeapon::ToggleWeaponScope(bool bUse)
{
	ScopeSceneCapturer->bCaptureOnMovement = bUse;
}

bool AGEMWeapon::UsesScopeSceneCapture() const
{
	if(!ScopeData)
	{
		return false;
	}

	return ScopeData->bUsesSceneCapture;
}

float AGEMWeapon::GetScopeFOVChange() const
{
	return ScopeADSFOV;
}

void AGEMWeapon::SetCurrentAmmo(int NewAmmoAmount)
{
	int32 OldAmmoCount = CurrentAmmoCount;
	CurrentAmmoCount = NewAmmoAmount;
	CurrentAmmoChanged.Broadcast(OldAmmoCount, CurrentAmmoCount);
}

void AGEMWeapon::SetMaximumAmmo(int NewAmmoAmount)
{
	int OldAmmoCount = MaxAmmoCount;
	MaxAmmoCount = NewAmmoAmount;
	MaxAmmoChanged.Broadcast(OldAmmoCount, MaxAmmoCount);
}

int AGEMWeapon::GetCurrentAmmo() const
{
	return CurrentAmmoCount;
}

int AGEMWeapon::GetMaximumAmmo() const
{
	return MaxAmmoCount;
}

float AGEMWeapon::GetRecoilHorizontal() const
{
	return RecoilHorizontal;
}

float AGEMWeapon::GetRecoilVertical() const
{
	return RecoilVertical;
}

float AGEMWeapon::GetScopeADSFOV() const
{
	return ScopeADSFOV;
}

float AGEMWeapon::GetPlayerADSFOV() const
{
	return PlayerADSFOV;
}

float AGEMWeapon::GetADSCameraOffset() const
{
	return ADSCameraOffset;
}

FGameplayTag AGEMWeapon::GetWeaponTag() const
{
	return WeaponData->WeaponTag;
}

UGEMWeaponData* AGEMWeapon::GetWeaponData() const
{
	return WeaponData;
}

UGEMAmmoData* AGEMWeapon::GetAmmoData() const
{
	return AmmoData;
}

UGEMScopeData* AGEMWeapon::GetScopeData() const
{
	return ScopeData;
}

TSubclassOf<UGameplayEffect> AGEMWeapon::GetAmmoConsumeClass() const
{
	return AmmoData->AmmoConsumeClass;
}

FGameplayAttribute AGEMWeapon::GetAmmoAttribute() const
{
	return AmmoData->AmmoAttribute;
}

FEffectsPack AGEMWeapon::GetOnFiredEffects() const
{
	return WeaponData->FireEffects;
}

FEffectsPack AGEMWeapon::GetBulletImpactEffects(FHitResult InHitResult) const
{
	AGEMCharacter* PlayerCharacter = Cast<AGEMCharacter>(InHitResult.GetActor());
	if(PlayerCharacter)
	{
		return WeaponData->ImpactEffects.PlayerHit;
	}
	
	return WeaponData->ImpactEffects.WorldHit;
}

UParticleSystem* AGEMWeapon::GetWeaponTrailEffect() const
{
	return WeaponData->WeaponTrail;
}

TSubclassOf<UCameraShakeBase> AGEMWeapon::GetOnFireCameraShake() const
{
	return WeaponData->OnFireCameraShake;
}

UAnimMontage* AGEMWeapon::ReloadMontage() const
{
	return WeaponData->ReloadMontage;
}

float AGEMWeapon::GetWeaponRange() const
{
	return WeaponData->EffectiveRange;
}

float AGEMWeapon::GetWeaponDamage() const
{
	return WeaponData->Damage;
}

void AGEMWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Only our owning actor needs to know about these
	DOREPLIFETIME_CONDITION(AGEMWeapon, CurrentAmmoCount, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AGEMWeapon, MaxAmmoCount, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AGEMWeapon, OwningCharacter, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AGEMWeapon, FireRate, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AGEMWeapon, PlayerADSFOV, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AGEMWeapon, ScopeADSFOV, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AGEMWeapon, ADSCameraOffset, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AGEMWeapon, RecoilVertical, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AGEMWeapon, RecoilHorizontal, COND_OwnerOnly);
	DOREPLIFETIME(AGEMWeapon, AmmoData);
	DOREPLIFETIME(AGEMWeapon, WeaponData);
	DOREPLIFETIME(AGEMWeapon, ScopeData);
}

void AGEMWeapon::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	DOREPLIFETIME_ACTIVE_OVERRIDE(AGEMWeapon, CurrentAmmoCount, (IsValid(AbilitySystemComponent) && !AbilitySystemComponent->HasMatchingGameplayTag(WeaponFiringTag)));
}

void AGEMWeapon::AddAbilities()
{

	// Make sure we are only being called on the server
	if(GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	
	if(!IsValid(OwningCharacter) || !OwningCharacter->GetAbilitySystemComponent())
	{
		GEM_PRINTSCREEN("Owning Character or Owning ASC is not valid")
		return;
	}
	
	if(!AbilitySystemComponent)
	{
		GEM_PRINTSCREEN("Not a valid GEM ASC")
		return;
	}

	for(TSubclassOf<UGEMGameplayAbility>& Ability : GrantedAbilities)
	{
		GrantedAbilitySpecHandles.Add(AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability,GetWeaponLevel(), static_cast<int32>(Ability.GetDefaultObject()->GetInputID()), this)));
	}
}

void AGEMWeapon::RemoveAbilities()
{
	
	// Make sure we are only being called on the server
	if(GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	
	if(!IsValid(OwningCharacter) || !OwningCharacter->GetAbilitySystemComponent())
	{
		GEM_PRINTSCREEN("Owning Character or Owning ASC is not valid")
		return;
	}
	
	if(!AbilitySystemComponent)
	{
		GEM_PRINTSCREEN("Not a valid GEM ASC")
		return;
	}

	// Loop through our Granted Ability Handles and Clear the Abilities
	for(FGameplayAbilitySpecHandle& Handle : GrantedAbilitySpecHandles)
	{
		AbilitySystemComponent->ClearAbility(Handle);
	}
}

int AGEMWeapon::GetWeaponLevel()
{
	return 1;
}

void AGEMWeapon::EquipWeapon()
{
	if(!OwningCharacter)
	{
		GEM_PRINTSCREEN("Owning Character is null pointer")
		return;
	}

	FName AttachPoint = OwningCharacter->GetWeaponAttachPoint();

	if(WeaponMeshFirstPerson && OwningCharacter->IsLocallyControlled())
	{
		WeaponMeshFirstPerson->AttachToComponent(OwningCharacter->GetFirstPersonMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachPoint);
		WeaponMeshFirstPerson->SetVisibility(true, true);
	}

	if(WeaponMeshFirstPerson && !OwningCharacter->IsLocallyControlled())
	{
		WeaponMeshFirstPerson->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachPoint);
		WeaponMeshFirstPerson->SetVisibility(true, true);
	}
	
}

void AGEMWeapon::UnequipWeapon()
{
	if(!OwningCharacter || !AbilitySystemComponent)
	{
		return;
	}
	
	Destroy();
}

void AGEMWeapon::SetOwningCharacter(AGEMCharacter* InCharacter)
{
	OwningCharacter = InCharacter;
	if(OwningCharacter)
	{
		AbilitySystemComponent = UGEMBlueprintFunctionLibrary::GetGEMAbilitySystemComponentFromActor(OwningCharacter);
		SetOwner(OwningCharacter);
	}

	else
	{
		AbilitySystemComponent = nullptr;
		SetOwner(nullptr);
	}
}

void AGEMWeapon::SetupData(const FInventoryWeaponData InWeaponData)
{
	UGEMWeaponData* DataAsset = InWeaponData.WeaponData;
	const int32 AmmoCount = InWeaponData.Ammo;
	FireRate = DataAsset->FireRate;
	MaxAmmoCount = DataAsset->AmmoCapacity;
	WeaponData = DataAsset;
	AmmoData = DataAsset->DefaultAmmoDataAsset;
	RecoilHorizontal = DataAsset->HorizontalRecoil;
	RecoilVertical = DataAsset->VerticalRecoil;
	SetScopeData(ScopeData, DataAsset->DefaultScopeDataAsset);
	CurrentAmmoCount = AmmoCount;
	// Append our Granted Abilities to our Weapon
	GrantedAbilities.Append(DataAsset->WeaponAbilities);
	GrantedAbilities.Append(DataAsset->DefaultScopeDataAsset->ScopeAbilities);
	
	// If our Ammo Count is -1, we should set out Ammo to the max amount
	if(AmmoCount == -1)
	{
		CurrentAmmoCount = MaxAmmoCount;
	}
}

void AGEMWeapon::SetScopeData(const UGEMScopeData* OldScopeData, UGEMScopeData* NewScopeData)
{
	if(OldScopeData == NewScopeData)
	{
		return;
	}
	
	// Incase for some reason in the future we make other weapon attributes change ADS, subtract the given ADS instead of just setting it to the new one
	if(OldScopeData)
	{
		PlayerADSFOV -= OldScopeData->PlayerADSFOV;
		ScopeADSFOV -= OldScopeData->ScopeADSFOV;
		ADSCameraOffset -= OldScopeData->CameraOffset;
	}

	// Same as above just adding the amount from our data asset
	if(NewScopeData)
	{
		PlayerADSFOV += NewScopeData->PlayerADSFOV;
		ScopeADSFOV += NewScopeData->ScopeADSFOV;
		ADSCameraOffset += NewScopeData->CameraOffset;
	}

	ScopeData = NewScopeData;
}

void AGEMWeapon::Client_SetDataAsset_Implementation(UGEMWeaponData* InWeaponData)
{
	WeaponData = InWeaponData;
}

void AGEMWeapon::SetValuesFromWeaponData()
{
	const FWeaponMeshData& MeshData = WeaponData->WeaponMesh;
	// Set our First Person Mesh
	GetSkeletalMeshFirstPerson()->SetSkeletalMesh(MeshData.SkeletalMesh);
	GetSkeletalMeshFirstPerson()->SetRelativeLocation(MeshData.LocationOffset);
	GetSkeletalMeshFirstPerson()->SetRelativeRotation(MeshData.RotationOffset);

	// Check if we have an animation blueprint we want to use with our weapon
	if(WeaponData->WeaponAnimationBlueprint)
	{
		GetSkeletalMeshFirstPerson()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetSkeletalMeshFirstPerson()->SetAnimInstanceClass(WeaponData->WeaponAnimationBlueprint);
	}

	// Check if we have any Subcomponents and add them to our Mesh
	if(WeaponData->AttachmentMeshes.Num() > 0)
	{
		for(const FSubWeaponMeshData Attachment : WeaponData->AttachmentMeshes)
		{
			switch(Attachment.AttachmentSlot)
			{
			case EWeaponAttachmentSlot::Stock:
				StockMesh->SetSkeletalMesh(Attachment.SkeletalMesh);
				StockMesh->SetRelativeLocation(Attachment.LocationOffset);
				StockMesh->SetRelativeRotation(Attachment.RotationOffset);
				break;
				
			case EWeaponAttachmentSlot::Handguard:
				HandguardMesh->SetStaticMesh(Attachment.StaticMesh);
				HandguardMesh->SetRelativeLocation(Attachment.LocationOffset);
				HandguardMesh->SetRelativeRotation(Attachment.RotationOffset);
				break;
			
			case EWeaponAttachmentSlot::Grip:
				break;
				
			case EWeaponAttachmentSlot::None:
				break;
				
			default: break;
			}
		}
	}
	// Scope Setup
	if(ScopeData && ScopeData->ScopeMeshData.Num() > 0)
	{
		for(const FSubWeaponMeshData Attachment : ScopeData->ScopeMeshData)
		{
			switch (Attachment.AttachmentSlot)
			{
				case EWeaponAttachmentSlot::Sight:
				SightMesh->AttachToComponent(WeaponMeshFirstPerson, FAttachmentTransformRules::SnapToTargetIncludingScale, Attachment.SocketAttachName);
				SightMesh->SetSkeletalMesh(Attachment.SkeletalMesh);
				SightMesh->SetRelativeLocation(Attachment.LocationOffset);
				SightMesh->SetRelativeRotation(Attachment.RotationOffset);
				break;

				case EWeaponAttachmentSlot::IronsightFront:
				IronsightFrontMesh->AttachToComponent(WeaponMeshFirstPerson, FAttachmentTransformRules::SnapToTargetIncludingScale, Attachment.SocketAttachName);
				IronsightFrontMesh->SetStaticMesh(Attachment.StaticMesh);
				IronsightFrontMesh->SetRelativeLocation(Attachment.LocationOffset);
				IronsightFrontMesh->SetRelativeRotation(Attachment.RotationOffset);
				break;

				case EWeaponAttachmentSlot::IronsightBack:
				IronsightFrontMesh->AttachToComponent(WeaponMeshFirstPerson, FAttachmentTransformRules::SnapToTargetIncludingScale, Attachment.SocketAttachName);
				IronsightFrontMesh->SetStaticMesh(Attachment.StaticMesh);
				IronsightFrontMesh->SetRelativeLocation(Attachment.LocationOffset);
				IronsightFrontMesh->SetRelativeRotation(Attachment.RotationOffset);
				break;

				default: break;
			}
		}
	}
	
	// BP PostSetup Functionality
	PostSetupWeaponData();
	
}


void AGEMWeapon::OnRep_CurrentAmmoCount(int32 OldCurrentAmmoCount)
{
	CurrentAmmoChanged.Broadcast(OldCurrentAmmoCount, CurrentAmmoCount);
}

void AGEMWeapon::OnRep_MaxAmmoCount(int32 OldMaxAmmoCount)
{
	MaxAmmoChanged.Broadcast(OldMaxAmmoCount, MaxAmmoCount);
}

// Called when the game starts or when spawned
void AGEMWeapon::BeginPlay()
{
	Super::BeginPlay();
}

