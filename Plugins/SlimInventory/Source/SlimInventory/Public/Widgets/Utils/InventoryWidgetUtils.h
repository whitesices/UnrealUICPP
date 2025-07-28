// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryWidgetUtils.generated.h"

/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API UInventoryWidgetUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static int32 GetInventoryIndexFromPosition( const FIntPoint& Position , const int32 Columns );
	//通过索引获取位置信息
	static FIntPoint GetPositionFromIndex( const int32 Index , const int32 Columns );
	//定义可供蓝图调用的获取UI位置的函数
	UFUNCTION(BlueprintCallable , Category="Inventory")
	static FVector2D GetWidgetPosition(UWidget* Widget);
};
