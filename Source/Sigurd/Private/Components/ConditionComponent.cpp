// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ConditionComponent.h"


// Sets default values for this component's properties
UConditionComponent::UConditionComponent(){

	ConditionHandler = CreateDefaultSubobject<UConditionHandler>(TEXT("ConditionHandler"));

}

void UConditionComponent::BeginPlay(){
	ConditionHandler->Initialize(this);
}

void UConditionComponent::AddBurn_Implementation(){
	ConditionHandler->StartBurning();
}

void UConditionComponent::AddExtinguish_Implementation(){
	ConditionHandler->Extinguish();
}

void UConditionComponent::AddElement_Implementation(Element NewElement){


	//if condition is none, add element to condition
	if (Conditions.Num() == 0){
		TFunction<void()>* Function = ConditionHandler->ConditionElementFunctionMap.Find(TPair<ECondition, Element>(ECondition::None, NewElement));
		if(Function){
			(*Function)();
		}
	}
	else{
		//if condition is not none, add element to condition
		for (ECondition Condition : Conditions){
			TFunction<void()>* Function = ConditionHandler->ConditionElementFunctionMap.Find(TPair<ECondition, Element>(Condition, NewElement));
			if(Function){
				(*Function)();
				return;
			}
		}
	}
	
}

