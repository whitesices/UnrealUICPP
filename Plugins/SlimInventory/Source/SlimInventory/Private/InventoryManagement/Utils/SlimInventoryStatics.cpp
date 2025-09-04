// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Utils/SlimInventoryStatics.h"
#include "InventoryManagement/Components/SlimInventoryComponent.h"

#include "Items/Components/SlimInventoryItemComponent.h"
#include "Types/InventoryGridTypes.h"//引入类型头文件
//#include "Items/Components/SlimInventoryItemComponent.h"
#include "Widgets/Inventory/InventoryBase/Slim_InventoryBase.h"//引入自定义的InventoryBase头文件

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

#pragma region HoverOperations
void USlimInventoryStatics::ItemHovered(APlayerController* PlayerController, USlimInventoryItem* InItem)
{
	//判断PlayerController与Item是否有效
	if (!IsValid(PlayerController))
	{
		return;
	}
	USlimInventoryComponent* CachedIncentoryComp = GetInventoryComponent(PlayerController);
	if (!IsValid(CachedIncentoryComp)) return;//判断是否有效
	USlim_InventoryBase* CacehdInventoryBase = CachedIncentoryComp->GetInventoryMenu();
	if (!IsValid(CacehdInventoryBase)) return;//判断是否有效
	
	if (CacehdInventoryBase->HasHoverItem()) return;//判断是否已经有HoverItem
	CacehdInventoryBase->OnItemHovered(InItem);//设置HoverItem

}

void USlimInventoryStatics::ItemUnhovered(APlayerController* PlayerController)
{
	USlimInventoryComponent* CachedInventoryComp = GetInventoryComponent(PlayerController);//获取InventoryComponet
	if (!IsValid(CachedInventoryComp)) return;//判断是否有效

	USlim_InventoryBase* CachedInventoryBase = CachedInventoryComp->GetInventoryMenu();
	if (!IsValid(CachedInventoryBase)) return;//判断是否有效

	CachedInventoryBase->OnItemUnhovered();
}
#pragma endregion