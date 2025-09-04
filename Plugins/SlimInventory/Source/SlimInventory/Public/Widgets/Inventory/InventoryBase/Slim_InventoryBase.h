// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/InventoryGridTypes.h"//引入自定的网格类型
#include "Slim_InventoryBase.generated.h"

class USlimInventoryItemComponent;
class USlimInventoryItem;//前置声明InventoryItem

/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API USlim_InventoryBase : public UUserWidget
{
	GENERATED_BODY()

public:
	//定义一个获取Item是否有充足空间的方法
	virtual FSlimSlotAvailabilityResult HasRoomForItem(USlimInventoryItemComponent* ItemComponent) const
	{
		return FSlimSlotAvailabilityResult();
	}
#pragma region HoverItem
	//定义Hovr与UnHover的方法
	virtual void OnItemHovered( USlimInventoryItem* Item ) {}
	virtual void OnItemUnhovered() {}
	//定义是否有HoverItem的方法
	virtual bool HasHoverItem() const { return false; }
#pragma endregion
};
