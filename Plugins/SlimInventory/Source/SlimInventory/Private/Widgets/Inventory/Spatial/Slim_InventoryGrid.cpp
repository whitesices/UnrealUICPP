// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/Slim_InventoryGrid.h"

#include "Widgets/Inventory/GridSlots/InventoryGridSlot.h"
#include "Widgets/Utils/InventoryWidgetUtils.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "InventoryManagement/Components/SlimInventoryComponent.h"
#include "Items/SlimInventoryItem.h"
#include "InventoryManagement/Utils/SlimInventoryStatics.h"
//引入小部件头文件
#include "Items/Components/SlimInventoryItemComponent.h"
#include "Items/Manifest/SlimItemManifest.h"

//引入插槽组件
#include "Widgets/Inventory/SlottedItems/SlimSlottedItem.h"
#include "Fragment/SlimItemFragment.h"
#include "Fragment/SlimFragmentTag.h"

void USlim_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//调用初始化网格函数
	ConstructGrid();

	//获取绑定的InventoryComponent
	InventoryComponent = USlimInventoryStatics::GetInventoryComponent( GetOwningPlayer() );
	InventoryComponent->OnItemAdded.AddDynamic( this ,&USlim_InventoryGrid::AddItem);
}

FSlimSlotAvailabilityResult USlim_InventoryGrid::HasRoomForItem(const USlimInventoryItemComponent* ItemComponent)
{
	return HasRoomForItem( ItemComponent->GetItemManifest() );//获取ItemManifest
}

void USlim_InventoryGrid::AddItemToIndices(const FSlimSlotAvailabilityResult& Result, USlimInventoryItem* NewItem)
{
	//Get Grid Fragment so we know how many grid spaces the item takes

	//Get Image Fragment so we have the icon to display

	//Create a widget to add to the grid

	//Store the new widget in container

	//遍历Slot有效性
	for (const auto& Avialability : Result.SlotAvailiabilites )
	{
		AddItemAtIndex( NewItem , Avialability.SlotIndex , Result.bStackable , Avialability.AmountToFill );//根据索引添加小部件
	}

}

void USlim_InventoryGrid::AddItemAtIndex(USlimInventoryItem* Item, const int32 index, const bool bStackable, const int32 StackAmount)
{
	const FSlimGridFragment* GridFragment = GetFragment<FSlimGridFragment>( Item , FragmentTags::FragmentTags_GridFragment);
	const FSlimImageFragment* ImageFragment = GetFragment<FSlimImageFragment>( Item , FragmentTags::FragmentTags_ImageFragment );

	//判断有效性
	if (!GridFragment || !ImageFragment) return;

	USlimSlottedItem* SlottedItem = CreateSlottedItem( Item , bStackable , StackAmount , GridFragment , ImageFragment , index );

	//Add the slotted item to the canvas panel

	//Store the new widget in a container
}

FVector2D USlim_InventoryGrid::GetDrawSize(const FSlimGridFragment* GridFragment) const
{
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2 ;
	return GridFragment->GetGridSize() * IconTileWidth;
}

//设置图片
void USlim_InventoryGrid::SetSlottedItemImage(const USlimSlottedItem* SlottedItem, const FSlimGridFragment* GridFragment, const FSlimImageFragment* ImageFragment) const
{
	FSlateBrush Brush;
	Brush.SetResourceObject( ImageFragment->GetIcon() );
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = GetDrawSize( GridFragment );
	SlottedItem->SetImageBrush(Brush);
}

USlimSlottedItem* USlim_InventoryGrid::CreateSlottedItem(USlimInventoryItem* Item, const bool bStackable, const int32 StackAmount, const FSlimGridFragment* GridFragment, const FSlimImageFragment* ImageFragment, const int32 index)
{
	USlimSlottedItem* SlottedItem = CreateWidget<USlimSlottedItem>( GetOwningPlayer() , SlottedItemClass );//创建UI
	//设置InventoryItem
	SlottedItem->SetInventoryItem( Item );
	//设置插槽图片
	SetSlottedItemImage( SlottedItem , GridFragment , ImageFragment );

	//设置网格索引
	SlottedItem->SetGridIndex(index);

	return SlottedItem;
}

void USlim_InventoryGrid::AddItem(USlimInventoryItem* Item)
{
	if (!MatchesCategory(Item))
	{
		return;
	}

	UE_LOG( LogTemp , Warning , TEXT("InventoryGrid:AddItem"));

	//返回对应的结果
	FSlimSlotAvailabilityResult Result = HasRoomForItem( Item );

	//Create  a  widget to show the item icon and  add it to the correct spot on the grid
	AddItemToIndices(Result , Item);

}

void USlim_InventoryGrid::ConstructGrid()
{
	GridSlots.Reserve( Rows * Columns );

	//循环遍历
	for (int32 i = 0 ; i < Columns; ++i )
	{
		for (int32 j = 0 ; j < Rows ; ++j )
		{
			//创建相应的网格
			UInventoryGridSlot* GridSlot = CreateWidget<UInventoryGridSlot>(this , GridSlotClass);
			CanvasPanel->AddChild( GridSlot );

			//创建声明对应的点位
			const FIntPoint TilePosition(i , j);
			GridSlot->SetTileIndex( UInventoryWidgetUtils::GetInventoryIndexFromPosition( TilePosition , Columns ) );

			//声明画布插槽
			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			//设置尺寸与位置信息
			GridCPS->SetSize( FVector2D(TileSize) );
			GridCPS->SetPosition( TilePosition * TileSize );

			GridSlots.Add(GridSlot);
		}
	}
}

bool USlim_InventoryGrid::MatchesCategory(const USlimInventoryItem* Item) const
{
	return Item->GetItemManifest().GetItemCatgory() == ItemCatgory;
}

FSlimSlotAvailabilityResult USlim_InventoryGrid::HasRoomForItem(const USlimInventoryItem* Item)
{
	return HasRoomForItem( Item->GetItemManifest() );
}

FSlimSlotAvailabilityResult USlim_InventoryGrid::HasRoomForItem(const FSlimItemManifest& Manifest)
{
	FSlimSlotAvailabilityResult Result;
	Result.TotalRoomToFill = 1;

	//申明插槽可见性
	FSlimInventorySlotVisibility SlotVisbility;
	SlotVisbility.AmountToFill = 1;
	SlotVisbility.SlotIndex = 0;
	
	Result.SlotAvailiabilites.Add( MoveTemp(SlotVisbility) );
	return Result;
}
