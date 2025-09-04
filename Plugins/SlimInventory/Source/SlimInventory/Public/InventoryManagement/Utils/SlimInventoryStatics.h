// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
//#include "Types/InventoryGridTypes.h"//引入类型名称
//引入UI蓝图函数库
#include "Widgets/Utils/InventoryWidgetUtils.h"

#include "SlimInventoryStatics.generated.h"

class USlimInventoryComponent;
class USlimInventoryItemComponent;
/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API USlimInventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//定义获取SlimInventory的库函数
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static USlimInventoryComponent* GetInventoryComponent(const APlayerController* PlayerController);

	//定义获取ItemCategory的全局函数
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static EInventory_ItemCategory GetItemCategoryFromItemComp(USlimInventoryItemComponent* ItemComp);

	//定义遍历矩阵的模板函数
	template< typename T, typename FuncT >
	static void ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns, const FuncT& Fucntion);

#pragma region HoverOperations
	UFUNCTION(BlueprintCallable , Category="Inventory")
	static void ItemHovered( APlayerController* PlayerController,USlimInventoryItem* InItem);

	UFUNCTION(BlueprintCallable , Category="Inventory")
	static void ItemUnhovered(APlayerController* PlayerController);
#pragma endregion
};

template<typename T, typename FuncT>
void USlimInventoryStatics::ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns, const FuncT& Fucntion)
{
	for ( int32 j = 0 ; j < Range2D.Y ; ++j )
	{
		for (int32 i = 0 ; i < Range2D.X ; ++i )
		{
			const FIntPoint Coordinates = UInventoryWidgetUtils::GetPositionFromIndex( Index , GridColumns ) + FIntPoint( i , j );
			UE_LOG(LogTemp , Warning , TEXT("x is %d , y is %d "), Coordinates.X , Coordinates.Y);
			const int32 TileIndex = UInventoryWidgetUtils::GetInventoryIndexFromPosition( Coordinates , GridColumns );

			//判断数组索引是否有效
			if ( Array.IsValidIndex(TileIndex) )
			{
				Fucntion( Array[TileIndex] );
			}
		}
	}
}
