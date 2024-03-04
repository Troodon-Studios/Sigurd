// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ConditionsComponent.h"

#include "Components/PlayerReactionHandler.h"


// Sets default values for this component's properties
UConditionsComponent::UConditionsComponent(){
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UConditionsComponent::BeginPlay(){
	Super::BeginPlay();

	//ExecuteReaction(Condition::None, Element::Fire);
	
}


// Called every frame
void UConditionsComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

