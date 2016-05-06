// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Messaging.h"

#include "RemoteJumpComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MESSAGINGSERVER_API URemoteJumpComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URemoteJumpComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	FMessageEndpointPtr JumpEndpoint;

	// Message handler for FJumpNowMessage, called by the Message Bus when a message arrives
	void JumpNowHandler(const struct FJumpNowMessage& Message, const IMessageContextRef& Context);

public:
	// declare a multicast delegate signature, since components have normal functions
	// to be assigned by an actor that uses the component.
	// the actual event that can be used from blueprint is declared later
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpNowSignature);

	// Blueprint function called when a message arrives
	UPROPERTY(BlueprintAssignable)
	FJumpNowSignature OnJumpNow;
};

USTRUCT()
struct FJumpNowMessage
{
	GENERATED_USTRUCT_BODY()
};
