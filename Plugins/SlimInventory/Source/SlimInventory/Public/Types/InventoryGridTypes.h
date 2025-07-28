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
	FSlimInventorySlotVisibility( int32 ItemIndex , int32 Room , bool bHasItem ):SlotIndex(ItemIndex),AmountToFill(Room),bItemAtIndex(bHasItem) {}

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

//创建图块象限类型
UENUM(BlueprintType)
enum class EInventoryTileQuadrant : uint8
{
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
	None
};

//创建图块参数类型
USTRUCT(BlueprintType)
struct FSlimInventoryTileParameters
{
	GENERATED_BODY()
public:
	//声明图块的宽高
	UPROPERTY(BlueprintReadWrite , EditAnywhere , Category="Inventory")
	FIntPoint TileCoordinates{};
	//声明图块的索引
	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category="Inventory")
	int32 TileIndex{INDEX_NONE};
	//声明图块的象限
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	EInventoryTileQuadrant TileQuadrant{ EInventoryTileQuadrant::None };

};

//创建空间查询的结果类
USTRUCT(BlueprintType)
struct FSlimSpaceQueryResult
{
	GENERATED_BODY()
public:
	//创建一个标志位来判断是否有空间
	/*UPROPERTY(BlueprintReadWrite , EditAnywhere , Category="Inventory")*/
	bool bHasSpace{ false };
	//创建一个变量来存储空间查询的结果存储有效的小部件
	TWeakObjectPtr<USlimInventoryItem> ValidItem = nullptr;
	//创建一个变量存储有效小部件左上角索引
	int32 UpperLeftIndex{ INDEX_NONE };
};

//重载==运算符
inline bool operator==( const FSlimInventoryTileParameters& A , const FSlimInventoryTileParameters& B )
{
	return A.TileCoordinates == B.TileCoordinates &&
		   A.TileIndex == B.TileIndex  &&
		   A.TileQuadrant == B.TileQuadrant;
}