// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ElementComponent.h"

#include "Components/ConditionsComponent.h"


// Sets default values for this component's properties
UElementComponent::UElementComponent(){
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UElementComponent::AddElement_Implementation(Element NewElement){
	if (ElementList.Contains(NewElement))
		// If the element is already in the list, return
		return;		
	
	// Add the new element to the list
	ElementList.Add(NewElement);

	// Iterate over the existing elements
	for (Element ExistingElement : ElementList) {
		// Check if there is a reaction between the existing element and the new element
		TPair<Element, Element> ElementPair(ExistingElement, NewElement);
		if (ReactionMap.Contains(ElementPair)) {
			// Get the reaction
			Reaction reaction = ReactionMap[ElementPair];

			// Perform the corresponding action based on the reaction
			switch (reaction) {
			case Reaction::Evaporate:
				// Perform the action for Evaporate
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Evaporate"));
				ElementList.Remove(ExistingElement);
				ElementList.Remove(NewElement);
				ElementList.Add(Element::Steam);
				//get condition component from owner
				GetOwner()->FindComponentByClass<UConditionsComponent>()->Conditions.Add(Condition::Burning);
				
				break;
			default:
				// Handle other reactions
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Unhandled reaction"));
	
				break;
			}
			break;
		}
	}
}

// Called when the game starts
void UElementComponent::BeginPlay(){
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UElementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

