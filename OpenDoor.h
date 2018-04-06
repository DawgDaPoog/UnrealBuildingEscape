// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseRequest);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FOnOpenRequest OnOpenRequest;
	UPROPERTY(BlueprintAssignable)
	FOnCloseRequest OnCloseRequest;
private:
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr; // Plate which opens the door via enough weight
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate2 = nullptr; // Plate on which you need to put a proper actor
	UPROPERTY(EditAnywhere)
	float TriggerMass = 20.f; // Weight threshhold
	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpens = nullptr; // Actor that opens the door from plate2
	FString GetActorNameOnPlate();
	AActor *Owner = nullptr; //Owning Door
	float GetTotalMassOfActorsOnPlate();
	void CheckDoorOpenedMethodOne(); // 2 pressure plates, one for weight one for a proper actor
	void CheckDoorOpenedMethodTwo(); // one pressure plate, only for weight
};
