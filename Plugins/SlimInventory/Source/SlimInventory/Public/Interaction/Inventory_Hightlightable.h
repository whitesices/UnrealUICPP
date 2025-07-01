// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Inventory_Hightlightable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventory_Hightlightable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLIMINVENTORY_API IInventory_Hightlightable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//声明高亮和取消高亮度接口函数
	UFUNCTION(BlueprintNativeEvent , Category="Inventory")
	void Highlight();
	UFUNCTION(BlueprintNativeEvent , Category="Inventory")
	void UnHightlight();
};
