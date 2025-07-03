// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"

#include "InventoryGridTypes.generated.h"

class USlimInventoryItem;
/**
 * 
 */
UENUM(BlueprintType)
enum class EInventory_ItemCategory : uint8
{
	Equippable,
	Consumable,
	Craftable,
	None
};

//声明插槽可见性结构体
USTRUCT()
struct FSlimInventorySlotVisibility
{
	GENERATED_BODY()

	//构造函数
	FSlimInventorySlotVisibility() {}
	//有参构造函数
	FSlimInventorySlotVisibility( int32 ItemIndex , int32 Room , bool bHasItem ):SlotIndex(ItemIndex),AmountToFill(ItemIndex),bItemAtIndex(bHasItem) {}

	//声明插槽可见性属性
	int32 SlotIndex = INDEX_NONE;
	int32 AmountToFill = 0;
	bool bItemAtIndex = false;
};

//声明插槽可见性的结果
USTRUCT()
struct FSlimSlotAvailabilityResult
{
	GENERATED_BODY()
	//构造函数
	FSlimSlotAvailabilityResult(){}

	//声明相应的属性
	TWeakObjectPtr<USlimInventoryItem> Item;
	int32 TotalRoomToFill = 0;
	int32 Remainder = 0;
	bool bStackable = false;
	TArray<FSlimInventorySlotVisibility> SlotAvailiabilites;

};