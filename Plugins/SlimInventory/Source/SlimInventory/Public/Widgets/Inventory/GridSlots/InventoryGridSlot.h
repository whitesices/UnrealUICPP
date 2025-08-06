// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGridSlot.generated.h"

class USlimInventoryItem;
class UImage;

//声明动态多播
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FGridSlotEvent , int32 , GridIndex , const FPointerEvent& , MouseEvent );

//新建新的插槽图片状态
UENUM(BlueprintType)
enum class ESlimGridSlotState : uint8
{
	Unoccupied,
	Occupied,
	Selected,
	GrayedOut
};


/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API UInventoryGridSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	//重载鼠标移动事件
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//设置小格子索引
	void SetTileIndex(int32 Index) { TileIndex = Index; }
	//获取TileIndex
	int32 GetTileIndex() const { return TileIndex; }

	void SetIndex(int32 Index) { TileIndex = Index; }
	int32 GetIndex() const { return TileIndex; }

	//获取网格插槽状态
	ESlimGridSlotState GetGridSlotState() const { return GridSlotState; }

	//设置或者获取相应的参数
	TWeakObjectPtr<USlimInventoryItem> GetInventoryItem() const { return InventoryItem; }
	void SetSlimInventoryItem( USlimInventoryItem* Item);

	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(int32 Count) { StackCount = Count; }

	int32 GetUpperLeftIndex() const { return UpperLeftIndex; }
	void SetUpperLeftIndex(int32 index) { UpperLeftIndex = index; }

	bool GetIsAvailable() const { return bAvailable; }
	void SetAvailable(bool bIsAvailble) { bAvailable = bIsAvailble; }

	//设置各个图片状态
	void SetOccupiedTexture();
	void SetUnoccupiedTexture();
	void SetSelectedTexture();
	void SetGrayedOutTexture();

	//声明委托绑定
	FGridSlotEvent GridSlotClicked;
	FGridSlotEvent GridSlotHovered;
	FGridSlotEvent GridSlotUnhovered;
	
private:
	int32 TileIndex{INDEX_NONE};
	//声明堆叠参数
	int32 StackCount{0};
	int32 UpperLeftIndex{INDEX_NONE};
	TWeakObjectPtr<USlimInventoryItem> InventoryItem;
	bool bAvailable{true};//是否有效

	UPROPERTY( meta=(BindWidget) )
	TObjectPtr<UImage> Image_GridSlot;

	//声明各个网格的刷新状态
	UPROPERTY( EditAnywhere , Category="Inventory" )
	FSlateBrush Brush_Unoccupied;
	UPROPERTY( EditAnywhere , Category="Inventory" )
	FSlateBrush Brush_Occupied;
	UPROPERTY( EditAnywhere , Category="Inventory")
	FSlateBrush Brush_Selected;
	UPROPERTY( EditAnywhere , Category="Inventory")
	FSlateBrush Brush_GrayedOut;

	//声明存储网格状态的变量
	ESlimGridSlotState GridSlotState;
};
