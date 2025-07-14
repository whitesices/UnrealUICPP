// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/SlottedItems/SlimSlottedItem.h"
#include "Items/SlimInventoryItem.h"//引入自定义的Item
#include "Components/Image.h" //引入图片

void USlimSlottedItem::SetImageBrush(const FSlateBrush& Brush) const
{
	//设置brush
	Image_Icon->SetBrush( Brush );
}

void USlimSlottedItem::SetInventoryItem(USlimInventoryItem* Item)
{
	InventoryItem = Item;
}
