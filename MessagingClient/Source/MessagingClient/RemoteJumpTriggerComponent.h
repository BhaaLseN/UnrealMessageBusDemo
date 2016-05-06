// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Messaging.h"
#include "RemoteJumpTriggerComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MESSAGINGCLIENT_API URemoteJumpTriggerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URemoteJumpTriggerComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	FMessageEndpointPtr JumpEndpoint;

public:
	// blueprints should call this function to trigger a remote jump
	UFUNCTION(BlueprintCallable, Category = "Remote Jump")
	void TriggerJump();
};

USTRUCT()
struct FJumpNowMessage
{
	GENERATED_USTRUCT_BODY()
};
