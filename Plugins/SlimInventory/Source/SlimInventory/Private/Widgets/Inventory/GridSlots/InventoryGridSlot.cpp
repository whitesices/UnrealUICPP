// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/GridSlots/InventoryGridSlot.h"
//引入图片组件头文件
#include "Components/Image.h"
#include "Items/SlimInventoryItem.h"
#include "Widgets/ItemPop/SlimItemPopUp.h"

void UInventoryGridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	GridSlotHovered.Broadcast( TileIndex , InMouseEvent );
}

void UInventoryGridSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	GridSlotUnhovered.Broadcast(TileIndex,InMouseEvent);
}

FReply UInventoryGridSlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	GridSlotClicked.Broadcast( TileIndex , InMouseEvent );
	return FReply::Handled();
}

void UInventoryGridSlot::SetSlimInventoryItem(USlimInventoryItem* Item)
{
	InventoryItem = Item;//设置相应的参数
}

void UInventoryGridSlot::SetItemPopup(USlimItemPopUp* PopUp)
{
	ItemPopup = PopUp;
	ItemPopup->SetGridIndex( GetIndex() );
	ItemPopup->OnNativeDestruct.AddUObject( this , &ThisClass::OnItemPopUpDestruct);//绑定析构函数
}

USlimItemPopUp* UInventoryGridSlot::GetItemPopup() const
{
	return ItemPopup.Get();
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

void UInventoryGridSlot::OnItemPopUpDestruct(UUserWidget* Menu)
{
	ItemPopup.Reset();
}
