// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/SlimInventoryItem.h"
//引入网络管理头文件
#include "Net/UnrealNetwork.h"
#include "Fragment/SlimItemFragment.h"


void USlimInventoryItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps( OutLifetimeProps );
	DOREPLIFETIME( ThisClass , ItemManiFest );
	DOREPLIFETIME( ThisClass , TotalStackCount );
}

void USlimInventoryItem::SetItemManifest(const FSlimItemManifest& Manifest)
{
	//获取条目清单
	ItemManiFest = FInstancedStruct::Make<FSlimItemManifest>(Manifest);
}

bool USlimInventoryItem::IsStackable() const
{
	const FSlimStackFragment* stackableFragment = GetItemManifest().GetFragmentOfType<FSlimStackFragment>();
	return stackableFragment != nullptr;
}

bool USlimInventoryItem::IsConsumable() const
{
	return GetItemManifest().GetItemCatgory() == EInventory_ItemCategory::Consumable;//将部件分类设定为消耗品
}
