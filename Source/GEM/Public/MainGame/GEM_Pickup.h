// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "GEM_Pickup.generated.h"

UCLASS()
class GEM_API AGEM_Pickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGEM_Pickup();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CollisionCapsule;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	

	// Required Tag name to use overlap event
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pickup)
	FName RequiredTag;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// All conditionals are met, this overlap event will fire
	UFUNCTION(BlueprintNativeEvent, Category = Pickup)
	void OverlapEvent(AActor* InActor);

	// Conditional Statement that must be true to run overlap event
	UFUNCTION(BlueprintNativeEvent, Category = Pickup)
	bool CanTriggerEvent(AActor* InActor);

	//*** Cooldown ***//
	// Whether we even fire cooldown events
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup | Cooldown")
	bool bUsesCooldown;
	// Cooldown timer for when the pickup will be made visible again
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup | Cooldown")
	float CooldownTime;

	UPROPERTY(ReplicatedUsing=OnRep_CooldownChanged, BlueprintReadWrite, Category = "Pickup | Cooldown")
	bool bCooldownActive;

	// Our OnRep Event for cooldown
	UFUNCTION()
	void OnRep_CooldownChanged();

	// What runs ONREP CooldownChanged in BP
	UFUNCTION(BlueprintNativeEvent, Category = "Pickup | Cooldown")
	void OnCooldownSet(bool IsCooldownOn);

	UFUNCTION(NetMulticast, Unreliable, Category = "Pickup | Cooldown")
	void Multicast_PlayEffects(AController* InController);

	UFUNCTION(BlueprintNativeEvent, Category = "Pickup | Cooldown")
	void PlayEffects(AController* InInstigatingController);

	UFUNCTION(BlueprintNativeEvent, Category = Pickup)
	void PostOverlapEvent();

	UFUNCTION()
	void OnCooldownTimerFinished();

	UFUNCTION(BlueprintNativeEvent, Category = "Pickup | Cooldown")
	void CooldownFinished();

	// Our cooldown timer handler
	FTimerHandle CooldownTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class UCapsuleComponent* GetCapsuleComponent() const { return CollisionCapsule; }
	
};
