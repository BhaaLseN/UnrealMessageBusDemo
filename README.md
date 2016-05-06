# UnrealMessageBusDemo
This is a sample application to illustrate the use of the [Unreal Message Bus system](https://docs.unrealengine.com/latest/INT/API/Runtime/Messaging/IMessageBus/index.html) from a native code project.

There are two UProject files included here:
- `MessagingServer` is the server application listening for `FJumpNowMessage` (which will cause the 3rd person character to jump).
- `MessagingClient` is the client application sending `FJumpNowMessage` after a simple UMG Button is clicked.

## How to run the Demo

1. Compile and package both projects for your platform (only tested on Windows, and the following steps will include Windows-specific instructions. If you use any other platform, you might have to adjust accordingly.)
2. Run the server application with the command line switch `-Messaging` to enable the Message Bus: `MessagingServer.exe -Messaging`
3. Run the client application with the command line switch `-Messaging` to enable the Message Bus: `MessagingClient.exe -Messaging`
4. With both applications running, click the "Jump" button in the client to see the Character in the server jump.

## About the Server
The interresting part of the server is [RemoteJumpComponent.cpp](blob/master/MessagingServer/Source/MessagingServer/RemoteJumpComponent.cpp).
It implements an Actor Component which is intended to be placed on a Character, which listens to its `OnJumpNow` Event to act accordingly (that is, to jump).

### Initialization
When `BeginPlay` is called, create an instance of `FMessageEndpoint` by using [`FMessageEndpoint::Builder()`](https://docs.unrealengine.com/latest/INT/API/Runtime/Messaging/Helpers/FMessageEndpointBuilder/index.html) with a given Endpoint name (which is only used for debugging purposes, and does not influence how message are delivered).

To indicate which messages the Endpoint handles, call the [`FMessageEndpointBuilder::Handling()`](https://docs.unrealengine.com/latest/INT/API/Runtime/Messaging/Helpers/FMessageEndpointBuilder/Handling/2/index.html) method and pass the message type as template parameter, along with the method to be called when such a message arrives.

This does _not_ mean message will be received, it only indicates what to do when a particular message arrives.
To actually receive them, call [`FMessageEndpoint::Subscribe<T>()`](https://docs.unrealengine.com/latest/INT/API/Runtime/Messaging/Helpers/FMessageEndpoint/Subscribe/1/index.html) for every message type that is interresting at this point.
This also means that you can dynamically decide whether you want to receive particular message types or not, and toggle them on or off using [`FMessageEndpoint.Unsubscribe<T>()`](https://docs.unrealengine.com/latest/INT/API/Runtime/Messaging/Helpers/FMessageEndpoint/Unsubscribe/2/index.html).

### Waiting for the Message
During initialization, a handler method has been registered with [`FMessageEndpointBuilder::Handling()`](https://docs.unrealengine.com/latest/INT/API/Runtime/Messaging/Helpers/FMessageEndpointBuilder/Handling/2/index.html) and enabled using [`FMessageEndpoint::Subscribe<T>()`](https://docs.unrealengine.com/latest/INT/API/Runtime/Messaging/Helpers/FMessageEndpoint/Subscribe/1/index.html).
As soon as a message arrives, this method will be called with 2 parameters:

1. The actual message.
2. A [Message Context](https://docs.unrealengine.com/latest/INT/API/Runtime/Messaging/IMessageContext/index.html) that stores additional data about and around the actual message.

You could use the [Message Context](https://docs.unrealengine.com/latest/INT/API/Runtime/Messaging/IMessageContext/index.html) to reply to the sender directly, but this is outside the scope of this sample.

Once the message arrives, do what you must to satisfy the request.

## About the Client
The interresting part of the client is [RemoteJumpTriggerComponent.cpp](blob/master/MessagingClient/Source/MessagingClient/RemoteJumpTriggerComponent.cpp).
Just like the server, it implements an Actor Component and is used on the HUD when the Button is clicked.

### Initialization
This is pretty much the same as for the Server. The only difference: We don't need to handle any messages, nor subscribe to them (unless we want to listen for them too).

### Sending a Message
Using the stored pointer to `FMessageEndpoint`, we can call its [`FMessageEndpoint::Publish<T>(T Message)`](https://docs.unrealengine.com/latest/INT/API/Runtime/Messaging/Helpers/FMessageEndpoint/Publish/1/index.html) method.
This is a **broadcast** and will go out to all listening entities on the Message Bus...or none at all, if noone is listening.

## About the Messages
A valid message must be a UStruct that contain any `UPROPERTY` type except for `UObjects`. This means integral types (`int32`, `float`, etc.), selected container types (`TArray`, `TMap`, etc.), and other `UStructs`.
It also needs a default constructor so it can be reconstructed on the other side.
