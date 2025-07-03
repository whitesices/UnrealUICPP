// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SlimInventoryStatics.generated.h"

class USlimInventoryComponent;

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
	
};
