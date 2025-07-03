// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/InventoryGridTypes.h"
#include "StructUtils/InstancedStruct.h"

#include "SlimItemManifest.generated.h"
/**
 * 
 */

class USlimInventoryItem;

USTRUCT(BlueprintType)
struct SLIMINVENTORY_API FSlimItemManifest
{
	GENERATED_BODY()

	USlimInventoryItem* Manifest( UObject* NewOuter);

	EInventory_ItemCategory GetItemCatgory() const
	{
		return ItemCategory;
	}

private:
	//定义选择的属性
	UPROPERTY( EditAnywhere , Category = "Inventory" )
	EInventory_ItemCategory ItemCategory{ EInventory_ItemCategory::None };
};