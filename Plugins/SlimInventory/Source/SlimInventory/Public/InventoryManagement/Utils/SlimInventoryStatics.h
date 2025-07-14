// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
//#include "Types/InventoryGridTypes.h"//引入类型名称
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
	UFUNCTION( BlueprintCallable , Category="Inventory")
	static USlimInventoryComponent* GetInventoryComponent( const APlayerController* PlayerController);

	//定义获取ItemCategory的全局函数
	UFUNCTION( BlueprintCallable , Category="Inventory")
	static EInventory_ItemCategory GetItemCategoryFromItemComp(USlimInventoryItemComponent* ItemComp);
};
