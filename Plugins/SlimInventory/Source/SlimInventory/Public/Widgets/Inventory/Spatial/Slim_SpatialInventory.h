// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/InventoryBase/Slim_InventoryBase.h"
#include "Slim_SpatialInventory.generated.h"

class USlim_InventoryGrid;
class UWidgetSwitcher;
class UButton;
/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API USlim_SpatialInventory : public USlim_InventoryBase
{
	GENERATED_BODY()
public:
	//重载初始化函数
	virtual void NativeOnInitialized() override;

private:
	//声明选择器小部件
	UPROPERTY( meta = (BindWidget) )
	TObjectPtr<UWidgetSwitcher> Switcher;
	//声明网格小部件
	UPROPERTY( meta = (BindWidget) )
	TObjectPtr<USlim_InventoryGrid> Grid_Equippables;
	
	UPROPERTY( meta = (BindWidget) )
	TObjectPtr<USlim_InventoryGrid> Grid_Consumables;

	UPROPERTY( meta = (BindWidget) )
	TObjectPtr<USlim_InventoryGrid> Grid_Craftables;

	//声明button小部件的绑定属性
	UPROPERTY( meta = (BindWidget) )
	TObjectPtr<UButton> Button_Equippables;
	UPROPERTY( meta = (BindWidget))
	TObjectPtr<UButton> Button_Consumables;
	UPROPERTY( meta = (BindWidget) )
	TObjectPtr<UButton> Button_Craftables;

	//声明显示的网格类型
	UFUNCTION()
	void ShowEquippables();
	UFUNCTION()
	void ShowConsumables();
	UFUNCTION()
	void ShowCraftables();

	//声明重置按钮状态函数
	void ResetButtonStates( UButton* Button );
	//声明激活对应网格函数
	void SetActivateGrid( USlim_InventoryGrid* Grid , UButton* Button );
	
};
