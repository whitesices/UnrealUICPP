// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Components/SlimInventoryItemComponent.h"

USlimInventoryItemComponent::USlimInventoryItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PickupMessage = FString("E - Pickup");
}




