// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Utils/SlimInventoryStatics.h"
#include "InventoryManagement/Components/SlimInventoryComponent.h"
//#include "Items/Components/SlimInventoryItemComponent.h"

USlimInventoryComponent* USlimInventoryStatics::GetInventoryComponent(const APlayerController* PlayerController)
{
	//检查PlayerController是否有效
	if (!IsValid(PlayerController)) return nullptr;
	USlimInventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<USlimInventoryComponent>();
	
	return InventoryComponent;
}
