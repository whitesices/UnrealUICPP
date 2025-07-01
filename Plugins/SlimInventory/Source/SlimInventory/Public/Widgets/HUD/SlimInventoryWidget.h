// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlimInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API USlimInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//声明显示信息的在蓝图中实现的方法
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventroy")
	void ShowPickupMessage( const FString& Message );

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void HidePickupMessage();
	
};
