// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/GridSlots/InventoryGridSlot.h"
//引入图片组件头文件
#include "Components/Image.h"
#include "Items/SlimInventoryItem.h"

void UInventoryGridSlot::SetSlimInventoryItem(USlimInventoryItem* Item)
{
	InventoryItem = Item;//设置相应的参数
}

void UInventoryGridSlot::SetOccupiedTexture()
{
	GridSlotState = ESlimGridSlotState::Occupied;
	Image_GridSlot->SetBrush(Brush_Occupied);
}

void UInventoryGridSlot::SetUnoccupiedTexture()
{
	GridSlotState = ESlimGridSlotState::Unoccupied;
	Image_GridSlot->SetBrush(Brush_Unoccupied);
}

void UInventoryGridSlot::SetSelectedTexture()
{
	GridSlotState = ESlimGridSlotState::Selected;
	Image_GridSlot->SetBrush(Brush_Selected);
}

void UInventoryGridSlot::SetGrayedOutTexture()
{
	GridSlotState = ESlimGridSlotState::GrayedOut;
	Image_GridSlot->SetBrush(Brush_GrayedOut);
}
