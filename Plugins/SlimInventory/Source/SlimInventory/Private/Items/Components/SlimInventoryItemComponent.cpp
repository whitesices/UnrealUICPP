// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Components/SlimInventoryItemComponent.h"
//引入网络管理头文件
#include "Net/UnrealNetwork.h"

USlimInventoryItemComponent::USlimInventoryItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PickupMessage = FString("E - Pickup");
}

void USlimInventoryItemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, ItemManifest);
}




