// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	//Find the owning actor
	Owner = GetOwner();
	if (!PressurePlate) UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"),*GetOwner()->GetName());
	if (!PressurePlate2) UE_LOG(LogTemp, Error, TEXT("%s missing optional pressure plate"), *GetOwner()->GetName());
	if (!ActorThatOpens) UE_LOG(LogTemp, Error, TEXT("%s missing actor that opens from additional pressure plate"), *GetOwner()->GetName());
}


	


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckDoorOpenedMethodOne();
	CheckDoorOpenedMethodTwo();
}
// Checking for the right actor on the plate2
FString UOpenDoor::GetActorNameOnPlate()
{
	TArray<AActor*> OverlappingActors;
	PressurePlate2->GetOverlappingActors(OUT OverlappingActors);
	for (auto* Actor : OverlappingActors)
		return *Actor->GetName();
	return "";
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return 0.f; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate throught masses
	for(auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	UE_LOG(LogTemp, Warning, TEXT("%s kilograms on pressure plate"), TotalMass);
	return TotalMass;
}

void UOpenDoor::CheckDoorOpenedMethodOne()
{
	if (ActorThatOpens && PressurePlate2) // First Mode of Opening a door (2 pressure plates, either you put enough weight on one to open the door, or put a specific actor on another plate to open)
	{
		if (GetTotalMassOfActorsOnPlate() > TriggerMass || GetActorNameOnPlate() == *ActorThatOpens->GetName())
		{
			OnOpenRequest.Broadcast();
		}
		else OnCloseRequest.Broadcast();
	}
}

void UOpenDoor::CheckDoorOpenedMethodTwo()
{
	if (!ActorThatOpens || !PressurePlate2) // Second mode (Only one plate, door is openable by weight)
	{
		if (GetTotalMassOfActorsOnPlate())
		{
			OnOpenRequest.Broadcast();
		}
		else OnCloseRequest.Broadcast();
	}
}
