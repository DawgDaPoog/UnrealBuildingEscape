// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Public/DrawDebugHelpers.h"
#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	/// Look for attached Physics Handle
	FindPhysicsHandleComponent();
	/// Look for attached Input Component
	FindInputHandleComponent();
}
/// Getting physics handle from the player
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}
}
/// Inputs and binding
void UGrabber::FindInputHandleComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		/// Binding axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName())
	}
}
///Counting the end for the trace
FVector UGrabber::GetLineTraceEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	FVector LineTraceEnd;
	LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
	return LineTraceEnd;
}

void UGrabber::Grab()
{
	///Try and reach any actors with physics body collision channel set
	auto HitResult = GetPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	/// If we hit something then attach a physics handle
	if (ActorHit) {
		if (!PhysicsHandle) return;
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true
		);
	}
}
void UGrabber::Release()
{
	// Release physics handle
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!PhysicsHandle) return;
	///if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)	
	{
		/// move object 
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}
/// Finding a body with gravity and physics in the range of reach
FHitResult UGrabber::GetPhysicsBodyInReach() 
{
	///Setup Query Params
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	///Ray-cast out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetWorld()->GetFirstPlayerController()->GetTargetLocation(), 
		GetLineTraceEnd(), 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	///See what we hit
	AActor* ActorHit = Hit.GetActor();
	return Hit;
}
