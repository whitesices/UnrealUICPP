// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"

#include "InventoryGridTypes.generated.h"

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