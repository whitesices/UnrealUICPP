// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/SlottedItems/SlimSlottedItem.h"
#include "Items/SlimInventoryItem.h"//引入自定义的Item
#include "Components/Image.h" //引入图片
#include "Components/TextBlock.h"//引入文本数据
#include "InventoryManagement/Utils/SlimInventoryStatics.h"//引入自定义的蓝图函数库

#pragma region 鼠标操作重载
FReply USlimSlottedItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnSlottedItemClicked.Broadcast(GridIndex , InMouseEvent );//触发自定义的多播委托
	return FReply::Handled();
}

void USlimSlottedItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	USlimInventoryStatics::ItemHovered( GetOwningPlayer(), InventoryItem.Get() );
}

void USlimSlottedItem::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	USlimInventoryStatics::ItemUnhovered( GetOwningPlayer() );
}
#pragma endregion

void USlimSlottedItem::SetImageBrush(const FSlateBrush& Brush) const
{
	//设置brush
	Image_Icon->SetBrush( Brush );
}

void USlimSlottedItem::SetInventoryItem(USlimInventoryItem* Item)
{
	InventoryItem = Item;
}

void USlimSlottedItem::UpdateStackCount(int32 StackCount)
{
	//判断StackCount是否大于零
	if (StackCount > 0)
	{
		Text_StackCount->SetVisibility( ESlateVisibility::SelfHitTestInvisible );
		Text_StackCount->SetText( FText::AsNumber(StackCount) );//设置文本数据
	}
	else
	{
		Text_StackCount->SetVisibility( ESlateVisibility::Collapsed );
	}
}
