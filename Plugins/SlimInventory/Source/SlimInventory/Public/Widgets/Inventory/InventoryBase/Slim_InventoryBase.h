// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/InventoryGridTypes.h"//引入自定的网格类型
#include "Slim_InventoryBase.generated.h"

class USlimInventoryItemComponent;

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
	
};
