// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/InventoryBase/Slim_InventoryBase.h"
#include "Slim_SpatialInventory.generated.h"

class USlim_InventoryGrid;
class UWidgetSwitcher;
class UButton;
class UCanvasPanel;
class USlimItemDescription;
/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API USlim_SpatialInventory : public USlim_InventoryBase
{
	GENERATED_BODY()
public:
#pragma region 重载父类函数
	//重载初始化函数
	virtual void NativeOnInitialized() override;
	//重载UI鼠标放下事件
	virtual FReply NativeOnMouseButtonDown( const FGeometry& MyGeometry , const FPointerEvent& MouseEvent) override;
	//重载Tick函数
	virtual void NativeTick( const FGeometry& MyGeometry , float InDeltaTime ) override;
#pragma endregion

	//重载父类的HasRoomForItem
	virtual FSlimSlotAvailabilityResult HasRoomForItem(USlimInventoryItemComponent* ItemComponent) const override;

	//Begin interface USlim_InventoryBase
	//重载父类的方法
	virtual void OnItemHovered(USlimInventoryItem* Item) override;
	virtual void OnItemUnhovered() override;
	virtual bool HasHoverItem() const override;
	//End interface USlim_InventoryBase

private:
	//添加画布
	UPROPERTY( meta = (BindWidget) )
	TObjectPtr<UCanvasPanel> CanvasPanel;

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
#pragma region ItemDescription
	//声明物品详细信息UI
	UPROPERTY(EditAnywhere , Category="Inventory")
	TSubclassOf<USlimItemDescription> ItemDescriptionClass;

	UPROPERTY()
	TObjectPtr<USlimItemDescription> ItemDescription;

	UPROPERTY(EditAnywhere , Category="Inventory")
	float DescriptionTimerDelay = 0.5f;

	//定义TimerHandle
	FTimerHandle DescriptionTimer;

#pragma endregion

	//声明激活的网格
	TWeakObjectPtr<USlim_InventoryGrid> ActivateGrid;

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

#pragma region ItemDescription
	USlimItemDescription* GetItemDescription();
	void SetItemDescriptionSizeAndPosition( USlimItemDescription* InItemDescription, UCanvasPanel* InCanvasPanel) const;
#pragma endregion
	
};
