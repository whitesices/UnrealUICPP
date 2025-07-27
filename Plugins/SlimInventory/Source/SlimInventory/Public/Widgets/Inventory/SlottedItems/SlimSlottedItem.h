// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlimSlottedItem.generated.h"

class USlimInventoryItem;
class UImage;
class UTextBlock;

//声明一个多播委托，用于处理小部件插槽点击事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FSlottedItemClicked , int32 , GridIndex , const FPointerEvent& , MouseEvent );

/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API USlimSlottedItem : public UUserWidget
{
	GENERATED_BODY()

public:
	//覆写button按下事件
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	//定义返回是否开启堆叠的函数
	bool IsStackable() const { return bIsStackable; }
	//定义设置是否开启堆叠的函数
	void SetIsStackable(bool bStackable) { bIsStackable = bStackable; }
	//获取图片资源的方法
	UImage* GetImageIcon() const { return Image_Icon; }
	//设置相应图片资源
	void SetImageBrush( const FSlateBrush& Brush) const;

	//获取GridIndex
	int32 GetGridIndex() { return GridIndex; }
	//设置GridIndex
	void SetGridIndex(int32 index) { GridIndex = index;  }

	//获取对应的网格维度
	FIntPoint GetGridDimension() { return GridDimensions;  }
	//设置对应的网格维度
	void SetGridDimension(FIntPoint Dimensions) { GridDimensions = Dimensions; }

	//设置自定义的InventoryItem
	void SetInventoryItem( USlimInventoryItem* Item );
	USlimInventoryItem* GetInventoryItem() const { return InventoryItem.Get(); }

	//更新栈文本数据
	void UpdateStackCount( int32 StackCount );

	FSlottedItemClicked OnSlottedItemClicked;//声明自定义的多播委托
	
private:
	//声明图片资源
	UPROPERTY( meta=(BindWidget) )
	TObjectPtr<UImage> Image_Icon;

	//声明文本数据进行绑定
	UPROPERTY( meta=(BindWidget) )
	TObjectPtr<UTextBlock> Text_StackCount;

	//相应的参数
	int32 GridIndex;
	FIntPoint GridDimensions;
	TWeakObjectPtr<USlimInventoryItem> InventoryItem;//声明Item
	bool bIsStackable{ false };//是否开启堆叠
	
};
