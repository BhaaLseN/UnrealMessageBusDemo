// Fill out your copyright notice in the Description page of Project Settings.

#include "MessagingClient.h"
#include "RemoteJumpTriggerComponent.h"

// Sets default values for this component's properties
URemoteJumpTriggerComponent::URemoteJumpTriggerComponent()
{
	bWantsBeginPlay = true;
}

// Called when the game starts
void URemoteJumpTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	// grab an endpoint so we can shout out to everyone listening, and hopefully reach the server
	JumpEndpoint = FMessageEndpoint::Builder("FRemoteJumpComponent").Build();
}

// Called when the game ends
void URemoteJumpTriggerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// release the message endpoint, to clean up resources.
	JumpEndpoint.Reset();

	Super::EndPlay(EndPlayReason);
}

void URemoteJumpTriggerComponent::TriggerJump()
{
	if (JumpEndpoint.IsValid())
		// this is a *broadcast*, it is not directed at any particular recipient
		JumpEndpoint->Publish<FJumpNowMessage>(new FJumpNowMessage());
}
