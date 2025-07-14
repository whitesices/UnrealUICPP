// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Utils/SlimInventoryStatics.h"
#include "InventoryManagement/Components/SlimInventoryComponent.h"

#include "Items/Components/SlimInventoryItemComponent.h"
#include "Types/InventoryGridTypes.h"//引入类型头文件
//#include "Items/Components/SlimInventoryItemComponent.h"

USlimInventoryComponent* USlimInventoryStatics::GetInventoryComponent(const APlayerController* PlayerController)
{
	//检查PlayerController是否有效
	if (!IsValid(PlayerController)) return nullptr;
	USlimInventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<USlimInventoryComponent>();
	
	return InventoryComponent;
}

EInventory_ItemCategory USlimInventoryStatics::GetItemCategoryFromItemComp(USlimInventoryItemComponent* ItemComp)
{
	//判断ItemComponent是否有效
	if (!IsValid(ItemComp)) return EInventory_ItemCategory::None;
	return ItemComp->GetItemManifest().GetItemCatgory();
}
