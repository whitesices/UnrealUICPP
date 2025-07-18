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
#include "Items/SlimInventoryItem.h"

#include "GameplayTagContainer.h"//引入GameplaytagContainer头文件

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
		UpdateGridSlots( NewItem , Avialability.SlotIndex , Result.bStackable , Avialability.AmountToFill );//更新网格数据
	}

}

void USlim_InventoryGrid::AddItemAtIndex(USlimInventoryItem* Item, const int32 index, const bool bStackable, const int32 StackAmount)
{
	//Get Grid Fragment so we know how many grid spaces the item takes
	const FSlimGridFragment* GridFragment = GetFragment<FSlimGridFragment>( Item , FragmentTags::FragmentTags_GridFragment);
	//Get Image Fragment so we have the icon to display
	const FSlimImageFragment* ImageFragment = GetFragment<FSlimImageFragment>( Item , FragmentTags::FragmentTags_ImageFragment );

	//判断有效性
	if (!GridFragment || !ImageFragment) return;
	//Create a widget to add to the grid
	USlimSlottedItem* SlottedItem = CreateSlottedItem( Item , bStackable , StackAmount , GridFragment , ImageFragment , index );

	//Add the slotted item to the canvas panel
	AddSlottedItemToCanvas( index , GridFragment , SlottedItem );
	//Store the new widget in a container
	SlottedItems.Add( index , SlottedItem );
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

	//设置是否可以堆叠,并显示相应的文本
	SlottedItem->SetIsStackable( bStackable );
	const int32 StackUpdateAmount = bStackable ? StackAmount : 0;
	SlottedItem->UpdateStackCount(StackUpdateAmount);

	return SlottedItem;
}

void USlim_InventoryGrid::AddSlottedItemToCanvas(const int32 Index, const FSlimGridFragment* GridFragment, USlimSlottedItem* SlottedItem) const
{
	CanvasPanel->AddChild(SlottedItem);
	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot( SlottedItem );
	CanvasSlot->SetSize( GetDrawSize(GridFragment) );//设置相应的尺寸
	const FVector2D DrawPos = UInventoryWidgetUtils::GetPositionFromIndex( Index , Columns ) * TileSize;
	const FVector2D DrawPosWithPadding = DrawPos + FVector2D( GridFragment->GetGridPadding() );
	//设置slottedItem在画布上的位置
	CanvasSlot->SetPosition( DrawPosWithPadding );
}

void USlim_InventoryGrid::UpdateGridSlots(USlimInventoryItem* NewItem, const int32 Index, bool bStackableItem, const int32 StackAmount)
{
	//检查网格插槽索引的有效性
	check(GridSlots.IsValidIndex(Index));

	////声明GridSlot
	//UInventoryGridSlot* GridSlot = GridSlots[Index];
	//GridSlot->SetOccupiedTexture();//设置网格插槽被占用的状态

	//遍历被占用的格子改变相应的选中状态
	//const FSlimGridFragment* GridFragment = GetFragment<FSlimGridFragment>(NewItem, FragmentTags::FragmentTags_GridFragment);
	////判断GridFragment是否有效
	//if (!GridFragment) return;
	//判断是否可以开始堆叠
	if ( bStackableItem )
	{
		GridSlots[Index]->SetStackCount(StackAmount);//将堆叠参数传入
	}

	//声明一个变量存储GridFragment
	const FSlimGridFragment* GridFragment = GetFragment<FSlimGridFragment>(NewItem, FragmentTags::FragmentTags_GridFragment);
	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);
	//遍历更新所占格子的颜色
	USlimInventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns, [&](UInventoryGridSlot* GridSlot)
	{
		GridSlot->SetOccupiedTexture();//设置该格子被占用
		GridSlot->SetSlimInventoryItem(NewItem);//设置新的小部件
		GridSlot->SetAvailable(false);
		GridSlot->SetUpperLeftIndex(Index);
	});
}

bool USlim_InventoryGrid::IsIndexClaimed(const TSet<int32>& CheckedIndices, const int32 index) const
{
	return CheckedIndices.Contains(index);//检查索引是否存在或者被占用
}

FSlimSlotAvailabilityResult USlim_InventoryGrid::HasRoomForItem(const FSlimItemManifest& Manifest)
{
	FSlimSlotAvailabilityResult Result;
	/*Result.TotalRoomToFill = 1;*/
	Result.TotalRoomToFill = 7;
	Result.bStackable = true;

	//申明插槽可见性
	FSlimInventorySlotVisibility SlotVisbility;
	/*SlotVisbility.AmountToFill = 1;*/
	SlotVisbility.AmountToFill = 2;
	SlotVisbility.SlotIndex = 0;

	Result.SlotAvailiabilites.Add(MoveTemp(SlotVisbility));

	//声明插槽可见性
	FSlimInventorySlotVisibility SlotAvailability2;
	SlotAvailability2.AmountToFill = 5;
	SlotAvailability2.SlotIndex = 1;
	Result.SlotAvailiabilites.Add(MoveTemp(SlotAvailability2));

	//Determine if the item is stackable
	const FSlimStackFragment* StackableFragment = Manifest.GetFragmentOfType<FSlimStackFragment>();
	Result.bStackable = StackableFragment != nullptr;//判断获取的片段是否有效
	//Determine how many stacks to add.
	const int32 MaxStackSize = StackableFragment ? StackableFragment->GetMaxStackSize() : 1;//获取最大堆叠数量
	int32 AmountToFill = StackableFragment ? StackableFragment->GetStackCount() : 1;//获取堆叠数量
	//声明一个集合存储已经检查的索引
	TSet<int32> CheckedIndices;
	//For each Grid Slot:
	for (const auto& GridSlot : GridSlots)
	{
		//if we don`t have anymore to fill , break out of the loop early.
		if (AmountToFill == 0) break;
		//Is this index claimed yet?
		if (IsIndexClaimed(CheckedIndices, GridSlot->GetIndex())) continue;

		//Can this item fit here?( i.e is it out of grid bounds?)
		//if (!HasRoomAtIndex( GridSlot , GetItemDimensions(Manifest) ) )
		//{
		//	continue;//if not enough room , continue to the next slot.
		//}
		//声明一个暂时变量存储
		TSet<int32> TentativeCheckedIndices;
		if (!HasRoomAtIndex(GridSlot, GetItemDimensions(Manifest), CheckedIndices, TentativeCheckedIndices , Manifest.GetItemType() ))
		{
			continue;//if not enough room , continue to the next slot.
		}

		//Check any other important conditions - ForEach2D over a 2D Range
		CheckedIndices.Append(TentativeCheckedIndices);

	}
	//if we don`t have anymore to fill , break out of the loop early.
   //Is this index claimed ye?
   //Can this item fit here?( i.e is it out of grid bounds?)
   //Check any other important conditions - ForEach2D over a 2D Range
	   //Index claimed?
	   //Has valid item?
	   //Is this item the same type as the we`re trying to add?
	   // If so , is this a stackable item?
	   // If stackable , is this slot at the max stack size already?
   //How much to fill?
   //Update the amount left to fill.
//How much is the Remainder?

	return Result;
}

bool USlim_InventoryGrid::HasRoomAtIndex(const UInventoryGridSlot* GridSlot, const FIntPoint& Dimensions, const TSet<int32>& CheckedIndices, TSet<int32>& OutTentativelyClaimed, const FGameplayTag& ItemType)
{
	//空间是否呗沾满
	bool bHasRoomAtIndex = true;//声明一个变量存储标志
	//遍历网格插槽数据
	USlimInventoryStatics::ForEach2D(GridSlots, GridSlot->GetIndex(), Dimensions, Columns, [&](const UInventoryGridSlot* SubGridSlot)
	{
		if (CheckSlotConstraints(GridSlot ,SubGridSlot , CheckedIndices , OutTentativelyClaimed ,ItemType ))
		{
			OutTentativelyClaimed.Add(SubGridSlot->GetIndex());
		}
		else
		{
			bHasRoomAtIndex = false;
		}


	});
	

	return bHasRoomAtIndex;//返回相应的状态
}

FIntPoint USlim_InventoryGrid::GetItemDimensions(const FSlimItemManifest& Manifest) const
{
	const FSlimGridFragment* GridFragment = Manifest.GetFragmentOfType<FSlimGridFragment>();
	return GridFragment ? GridFragment->GetGridSize() : FIntPoint(1 ,1 ) ;
}

bool USlim_InventoryGrid::CheckSlotConstraints(const UInventoryGridSlot* GridSlot, const UInventoryGridSlot* SubGridSlot, const TSet<int32>& CheckedIndecies, TSet<int32>& OutTentativelyClaimed, const FGameplayTag& ItemType) const
{
	//Index claimed?
	if (IsIndexClaimed(CheckedIndecies, SubGridSlot->GetIndex())) return false;
			//Has valid item?
	if (!HasValidItem(SubGridSlot))
	{
		OutTentativelyClaimed.Add( SubGridSlot->GetIndex() );
		return true;
	}
	//Is this Grid Slot an upper left slot?
	if (!IsUpperLeftSlot(GridSlot, SubGridSlot)) return false;
	
	// If so , is this a stackable item?
	const USlimInventoryItem* SubItem = SubGridSlot->GetInventoryItem().Get();
	if ( !SubItem->IsStackable() ) return false;

	//Is this item the same type as the we`re trying to add?
	if (!DoesItemTypeMatch(SubItem, ItemType)) return false;
	// If stackable , is this slot at the max stack size already?

	return false;
}

bool USlim_InventoryGrid::HasValidItem(const UInventoryGridSlot* GridSlot) const
{
	return GridSlot->GetInventoryItem().IsValid();
}

bool USlim_InventoryGrid::IsUpperLeftSlot(const UInventoryGridSlot* GridSlot, const UInventoryGridSlot* SubGridSlot) const
{
	return SubGridSlot->GetUpperLeftIndex() == GridSlot->GetIndex();
}

bool USlim_InventoryGrid::DoesItemTypeMatch(const USlimInventoryItem* SubItem, const FGameplayTag& ItemType) const
{
	return SubItem->GetItemManifest().GetItemType().MatchesTagExact(ItemType);
}

//void USlim_InventoryGrid::UpdateGridSlots(USlimInventoryItem* NewItem, const int32 Index)
//{
//	//检查网格插槽索引的有效性
//	check( GridSlots.IsValidIndex(Index) );
//
//	////声明GridSlot
//	//UInventoryGridSlot* GridSlot = GridSlots[Index];
//	//GridSlot->SetOccupiedTexture();//设置网格插槽被占用的状态
//
//	//遍历被占用的格子改变相应的选中状态
//	const FSlimGridFragment* GridFragment = GetFragment<FSlimGridFragment>( NewItem , FragmentTags::FragmentTags_GridFragment );
//	//判断GridFragment是否有效
//	if (!GridFragment) return;
//
//	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint( 1, 1 );
//	//遍历更新所占格子的颜色
//	USlimInventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns, []( UInventoryGridSlot* GridSlot)
//	{
//		GridSlot->SetOccupiedTexture();//设置该格子被占用
//	});
//}

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
	for (int32 j = 0 ; j < Rows; ++j )
	{
		for (int32 i = 0 ; i < Columns; ++i )
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


