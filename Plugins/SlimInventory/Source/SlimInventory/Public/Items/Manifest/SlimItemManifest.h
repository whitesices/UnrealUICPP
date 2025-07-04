// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/InventoryGridTypes.h"
#include "StructUtils/InstancedStruct.h"
//引入gameplayTag
#include "GameplayTagContainer.h"

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

	//返回物品的标签
	FGameplayTag GetItemType() const
	{
		return ItemType;
	}

private:
	//定义选择的属性
	UPROPERTY( EditAnywhere , Category = "Inventory" )
	EInventory_ItemCategory ItemCategory{ EInventory_ItemCategory::None };

	//定义物品的标签
	UPROPERTY( EditAnywhere , Category = "Inventory" )
	FGameplayTag ItemType;
};