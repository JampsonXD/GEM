// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGame/GEM_Pickup.h"

// Sets default values
AGEM_Pickup::AGEM_Pickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Capsule"));

	// Set the root component as our collision
	CollisionCapsule->SetupAttachment(RootComponent);
	CollisionCapsule->SetGenerateOverlapEvents(true);
	
	// Setting Defaults
	// Set to replicate
	bReplicates = true;
	bUsesCooldown = true;
	bCooldownActive = false;
	CooldownTime = 10.f;
}

void AGEM_Pickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Here we list the variables we want to replicate + a condition if wanted
	DOREPLIFETIME(AGEM_Pickup, bCooldownActive);
}

// Called when the game starts or when spawned
void AGEM_Pickup::BeginPlay()
{
	Super::BeginPlay();
}

void AGEM_Pickup::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Init our Collision Capsules Overlap Event
	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AGEM_Pickup::OnOverlapBegin);
	
}

// Our overlap event
void AGEM_Pickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Only run our checks on the server
	if(HasAuthority())
	{
		if(CanTriggerEvent(OtherActor))
		{
			OverlapEvent(OtherActor);
			Multicast_PlayEffects(OtherActor->GetInstigatorController());
			// Multicast for any VFX/SFX done in blueprint
			if(bUsesCooldown)
			{
				bCooldownActive = true;
				OnRep_CooldownChanged();
				// Set timer if we use a cooldown on this
				GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &AGEM_Pickup::OnCooldownTimerFinished, CooldownTime, false);
			}
			else
			{
				// If we dont have any cooldown events happening, we can just run a Post event to deal with any extra functionality we want, like destroying the actor
				PostOverlapEvent();
			}
		}
	}
}

void AGEM_Pickup::OverlapEvent_Implementation(AActor* InActor)
{
	
}

bool AGEM_Pickup::CanTriggerEvent_Implementation(AActor* InActor)
{
	return true;
}

// Multicast for effects played on Overlap success
void AGEM_Pickup::Multicast_PlayEffects_Implementation(AController* InController)
{
	PlayEffects(InController);
}

void AGEM_Pickup::PlayEffects_Implementation(AController* InInstigatingController)
{
}

void AGEM_Pickup::OnRep_CooldownChanged()
{
	OnCooldownSet(bCooldownActive);
}

void AGEM_Pickup::PostOverlapEvent_Implementation()
{
}

void AGEM_Pickup::OnCooldownTimerFinished()
{
	bCooldownActive = false;
	OnRep_CooldownChanged();
	// Post cooldown function, any functionality run after the cooldown is finished and we are turning the collision for pickups back on
	CooldownFinished();
}

void AGEM_Pickup::CooldownFinished_Implementation()
{
	
}

void AGEM_Pickup::OnCooldownSet_Implementation(bool IsCooldownOn)
{
	
}

// Called every frame
void AGEM_Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

