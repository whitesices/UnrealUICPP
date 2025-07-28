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
#include "SlimInventory.h"

#include "GameplayTagContainer.h"//引入GameplaytagContainer头文件
#include "Widgets/Inventory/HoverItem/SlimHoverItem.h"

#pragma region 覆写函数
void USlim_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//调用初始化网格函数
	ConstructGrid();

	//获取绑定的InventoryComponent
	InventoryComponent = USlimInventoryStatics::GetInventoryComponent( GetOwningPlayer() );
	InventoryComponent->OnItemAdded.AddDynamic( this ,&USlim_InventoryGrid::AddItem);
	InventoryComponent->OnStackChange.AddDynamic( this , &USlim_InventoryGrid::AddStackNumer);
}

void USlim_InventoryGrid::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const FVector2D CanvasPosition = UInventoryWidgetUtils::GetWidgetPosition( CanvasPanel );
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport( GetOwningPlayer() );

	//调用更新图块操作
	UpdateTileParameters( CanvasPosition , MousePosition );
}
#pragma endregion

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

	//小部件绑定事件
	SlottedItem->OnSlottedItemClicked.AddDynamic( this , &ThisClass::OnSlottedItemClicked);

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
	//Result.TotalRoomToFill = 7;
	//Result.bStackable = true;

	////申明插槽可见性
	//FSlimInventorySlotVisibility SlotVisbility;
	///*SlotVisbility.AmountToFill = 1;*/
	//SlotVisbility.AmountToFill = 2;
	//SlotVisbility.SlotIndex = 0;

	//Result.SlotAvailiabilites.Add(MoveTemp(SlotVisbility));

	////声明插槽可见性
	//FSlimInventorySlotVisibility SlotAvailability2;
	//SlotAvailability2.AmountToFill = 5;
	//SlotAvailability2.SlotIndex = 1;
	//Result.SlotAvailiabilites.Add(MoveTemp(SlotAvailability2));

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

		//Is the item in grid bouds?
		if (!IsInGridBounds( GridSlot->GetIndex() , GetItemDimensions(Manifest) )) continue;

		//Can this item fit here?( i.e is it out of grid bounds?)
		//if (!HasRoomAtIndex( GridSlot , GetItemDimensions(Manifest) ) )
		//{
		//	continue;//if not enough room , continue to the next slot.
		//}
		//声明一个暂时变量存储
		TSet<int32> TentativeCheckedIndices;
		if (!HasRoomAtIndex(GridSlot, GetItemDimensions(Manifest), CheckedIndices, TentativeCheckedIndices , Manifest.GetItemType() , MaxStackSize ))
		{
			continue;//if not enough room , continue to the next slot.
		}

		//Check any other important conditions - ForEach2D over a 2D Range
		/*CheckedIndices.Append(TentativeCheckedIndices);*/

		
		const int32 AmountToFillInslot = DetermineFilAmountForSlot( Result.bStackable , MaxStackSize , AmountToFill , GridSlot );
		if (AmountToFillInslot == 0) continue;//If we can`t fill this slot , continue to the next one.

		CheckedIndices.Append(TentativeCheckedIndices);

		//Update the amount left to fill.
		Result.TotalRoomToFill += AmountToFillInslot;
		Result.SlotAvailiabilites.Emplace(
			FSlimInventorySlotVisibility{
				HasValidItem(GridSlot) ? GridSlot->GetUpperLeftIndex() : GridSlot->GetIndex(),
				Result.bStackable ? AmountToFillInslot : 1,
				HasValidItem(GridSlot)
			}
		);

		AmountToFill -= AmountToFillInslot;//更新剩余填充数量
		//How much to fill?
		Result.Remainder = AmountToFill;//更新剩余数量

		if (AmountToFill == 0) return Result;

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

bool USlim_InventoryGrid::HasRoomAtIndex(const UInventoryGridSlot* GridSlot,
	const FIntPoint& Dimensions, 
	const TSet<int32>& CheckedIndices, 
	TSet<int32>& OutTentativelyClaimed, 
	const FGameplayTag& ItemType,
	const int32 MaxStackSize)
{
	//空间是否呗沾满
	bool bHasRoomAtIndex = true;//声明一个变量存储标志
	//遍历网格插槽数据
	USlimInventoryStatics::ForEach2D(GridSlots, GridSlot->GetIndex(), Dimensions, Columns, [&](const UInventoryGridSlot* SubGridSlot)
	{
		if (CheckSlotConstraints(GridSlot ,SubGridSlot , CheckedIndices , OutTentativelyClaimed ,ItemType, MaxStackSize))
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

bool USlim_InventoryGrid::CheckSlotConstraints(
	const UInventoryGridSlot* GridSlot, 
	const UInventoryGridSlot* SubGridSlot, 
	const TSet<int32>& CheckedIndecies, 
	TSet<int32>& OutTentativelyClaimed, 
	const FGameplayTag& ItemType,
	const int32 MaxStackSize) const
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
	if (GridSlot->GetStackCount() >= MaxStackSize) return false;

	return true;
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

bool USlim_InventoryGrid::IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const
{
	if (StartIndex < 0 || StartIndex >= GridSlots.Num()) return false;//判断检查索引是否有效
	const int32 EndColumn = ( StartIndex % Columns ) + ItemDimensions.X;
	const int32 EndRow = (StartIndex / Columns ) + ItemDimensions.Y;

	return EndColumn <= Columns && EndRow <= Rows;//返回是否在网格边界内
}

int32 USlim_InventoryGrid::DetermineFilAmountForSlot(const bool bStackable, 
	const int32 MaxStackSize, const int32 AmountToFill, const UInventoryGridSlot* GridSlot) const
{
	//计算插槽空间
	const int32 RoomInSlot = MaxStackSize - GetStackAmount( GridSlot );
	return bStackable ? FMath::Min( AmountToFill , RoomInSlot ) : 1;
}

int32 USlim_InventoryGrid::GetStackAmount(const UInventoryGridSlot* GridSlot) const
{
	int32 CurrentSlotStackAmount = GridSlot->GetStackCount();
	//If we are at a slot that doesn`t hold the stack count , we must get the actual stack count
	if (const int32 UpperLeftIndex = GridSlot->GetUpperLeftIndex() ; UpperLeftIndex != INDEX_NONE )
	{
		UInventoryGridSlot* UpperLeftSlot = GridSlots[UpperLeftIndex];
		CurrentSlotStackAmount = UpperLeftSlot->GetStackCount();//获取当前的堆叠数量
	}


	return CurrentSlotStackAmount;
}

#pragma region 鼠标事件处理函数
void USlim_InventoryGrid::OnSlottedItemClicked(int32 InGridIndex, const FPointerEvent& InMouseEvent)
{
	/*UE_LOG(LogTemp, Warning, TEXT("Clicked on item at index %d"), InGridIndex);*/
	//判断网格插槽索引有效
	check( GridSlots.IsValidIndex(InGridIndex) );

	//获取点击的网格插槽小部件
	USlimInventoryItem* ClickedItem = GridSlots[InGridIndex]->GetInventoryItem().Get();
	//判断Hover事件是否有效
	if (!IsValid(HoverItem) && IsLeftClick(InMouseEvent) )
	{
		//TODO: PickUp - Assign the hover item , and remove the slotted item from the grid
		PickUp(ClickedItem, InGridIndex);
	}
}

bool USlim_InventoryGrid::IsRightClick(const FPointerEvent& InMouseEvent) const
{
	return InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton;
}

bool USlim_InventoryGrid::IsLeftClick(const FPointerEvent& InMouseEvent) const
{
	return InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
}
void USlim_InventoryGrid::PickUp(USlimInventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
	//Assign hover item
	AssignHoverItem(ClickedInventoryItem, GridIndex, GridIndex);
	//Remove Clicked Item from the grid
	RemoveItemFromGrid(ClickedInventoryItem , GridIndex );
}
void USlim_InventoryGrid::AssignHoverItem(USlimInventoryItem* InventoryItem)
{
	if (!IsValid(HoverItem))
	{
		HoverItem = CreateWidget<USlimHoverItem>( GetOwningPlayer() , HoverItemClass );
	}

	const FSlimGridFragment* GridFragment = GetFragment<FSlimGridFragment>(InventoryItem , FragmentTags::FragmentTags_GridFragment);
	const FSlimImageFragment* ImageFragment = GetFragment<FSlimImageFragment>(InventoryItem, FragmentTags::FragmentTags_ImageFragment);

	if (!GridFragment || !ImageFragment) return;

	const FVector2D DrawSize = GetDrawSize(GridFragment);

	FSlateBrush ImageBrush;
	ImageBrush.SetResourceObject( ImageFragment->GetIcon() );
	ImageBrush.DrawAs = ESlateBrushDrawType::Image;
	ImageBrush.ImageSize = DrawSize * UWidgetLayoutLibrary::GetViewportScale(this);//图片尺寸的缩放

	HoverItem->SetImageBrush(ImageBrush);//设置HoverItem的图片
	HoverItem->SetGridDimensions(GridFragment->GetGridSize() );//设置HoverItem的网格尺寸
	HoverItem->SetInventoryItem(InventoryItem);
	HoverItem->SetIsStackable( InventoryItem->IsStackable() );//设置HoverItem是否可以堆叠

	GetOwningPlayer()->SetMouseCursorWidget( EMouseCursor::Default , HoverItem );//设置鼠标光标小部件
}
void USlim_InventoryGrid::AssignHoverItem(USlimInventoryItem* InventoryItem, const int32 GridIndex, const int32 PreviousGridIndex)
{
	AssignHoverItem(InventoryItem);

	HoverItem->SetPreviousGridIndex( PreviousGridIndex );//设定鼠标悬停之前的网格索引
	HoverItem->UpdateStackCountText( InventoryItem->IsStackable() ? GridSlots[GridIndex]->GetStackCount() : 0 );//更新堆叠数量文本
}
void USlim_InventoryGrid::RemoveItemFromGrid(USlimInventoryItem* InventoryItem, const int32 GridIndex)
{
	const FSlimGridFragment* GridFragment = GetFragment<FSlimGridFragment >( InventoryItem , FragmentTags::FragmentTags_GridFragment );

	if (!GridFragment) return;

	USlimInventoryStatics::ForEach2D(GridSlots, GridIndex, GridFragment->GetGridSize(), Columns, [&]( UInventoryGridSlot* GridSlot )
	{
		GridSlot->SetSlimInventoryItem(nullptr);//清除网格插槽小部件
		GridSlot->SetUpperLeftIndex(INDEX_NONE);//清除左上角索引
		GridSlot->SetUnoccupiedTexture();//设置网格插槽未被占用的状态
		GridSlot->SetAvailable(true);//设置插槽可用状态
		GridSlot->SetStackCount(0);//清除堆叠数量
	});//遍历更新Item信息

	if ( SlottedItems.Contains(GridIndex ) )
	{
		TObjectPtr<USlimSlottedItem> FoundSlottedItem;
		SlottedItems.RemoveAndCopyValue( GridIndex , FoundSlottedItem );
		FoundSlottedItem->RemoveFromParent();//从父级移除小部件
	}
}
void USlim_InventoryGrid::UpdateTileParameters(const FVector2D& CanavsPosition, const FVector2D& MousePosition)
{
	//if mouse not in canvas , return
	//Calculate the tile quadrant,title index , and coordinates
	const FIntPoint HoverItemCoordinates = CalculateHoverItemSize(CanavsPosition, MousePosition);

	LastTileParameters = TileParameters;
	TileParameters.TileCoordinates = HoverItemCoordinates;
	TileParameters.TileIndex = UInventoryWidgetUtils::GetInventoryIndexFromPosition( HoverItemCoordinates,Columns );
	TileParameters.TileQuadrant = CalculateTileQuadrant(CanavsPosition, MousePosition);
	//Handle highlight/Unhighlight of the tile
	OnTileParametersUpdated(TileParameters);
}

//计算鼠标悬停图块大小
FIntPoint USlim_InventoryGrid::CalculateHoverItemSize(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const
{
	return FIntPoint{
		static_cast<int32>( FMath::FloorToInt( ( MousePosition.X - CanvasPosition.X ) / TileSize ) ),
		static_cast<int32>( FMath::FloorToInt( (MousePosition.Y - CanvasPosition.Y ) / TileSize ) )
	};
}
EInventoryTileQuadrant USlim_InventoryGrid::CalculateTileQuadrant(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const
{
	//Calculate relative position within the current tile
	const float TileLocalX = FMath::Fmod( MousePosition.X - CanvasPosition.X , TileSize );
	const float TileLocalY = FMath::Fmod( MousePosition.Y - CanvasPosition.Y , TileSize );

	//Determine which quarant the mouse is in
	const bool bIsTop = TileLocalY < TileSize / 2.f;//Top if Y is in the upper half
	const bool bIsLeft = TileLocalX < TileSize / 2.f;//Left if X is in the left half

	EInventoryTileQuadrant HoveredTileQuardrant{EInventoryTileQuadrant::None};
	if (bIsTop && bIsLeft)
	{
		HoveredTileQuardrant = EInventoryTileQuadrant::TopLeft;
	}
	else if (bIsTop && !bIsLeft)
	{
		HoveredTileQuardrant = EInventoryTileQuadrant::TopRight;
	}
	else if (!bIsTop && bIsLeft)
	{
		HoveredTileQuardrant = EInventoryTileQuadrant::BottomLeft;
	}
	else if (!bIsTop && !bIsLeft)
	{
		HoveredTileQuardrant = EInventoryTileQuadrant::BottomRight;
	}

	return HoveredTileQuardrant;
}
void USlim_InventoryGrid::OnTileParametersUpdated(const FSlimInventoryTileParameters& Parameters)
{
	if (!IsValid(HoverItem)) return;

	//Get HoverItem`s dimensions
	const FIntPoint HoverItemDimensions = HoverItem->GetGridDimensions();
	//Calculate the starting coordinates for the hover item
	const FIntPoint StartPoint = CalculateStartPoint(Parameters.TileCoordinates , HoverItemDimensions , Parameters.TileQuadrant );//计算初始起点
	ItemDropIndex = UInventoryWidgetUtils::GetInventoryIndexFromPosition( StartPoint , Columns );//通过位置获取网格索引
	
	CurrentSpaceQueryResult = CheckHoverPosition( StartPoint , HoverItemDimensions );
}
FIntPoint USlim_InventoryGrid::CalculateStartPoint(const FIntPoint& Coordinate, const FIntPoint& Dimensions, const EInventoryTileQuadrant Quadrant) const
{
	const int32 HasEvenWidth = Dimensions.X % 2 == 0 ? 1 : 0;//判断宽度是否为偶数
	const int32 HasEvenHeight = Dimensions.Y % 2 == 0 ? 1 : 0; //判断高度是否为偶数

	FIntPoint StartPoint;

	switch (Quadrant)
	{
	case EInventoryTileQuadrant::TopLeft:
					StartPoint.X = Coordinate.X - FMath::FloorToInt( 0.5f * Dimensions.X );
					StartPoint.Y = Coordinate.Y - FMath::FloorToInt( 0.5f * Dimensions.Y );
					break;
	case EInventoryTileQuadrant::TopRight:
		StartPoint.X = Coordinate.X - FMath::FloorToInt( 0.5f * Dimensions.X ) + HasEvenWidth;
		StartPoint.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y);
		break;
	case EInventoryTileQuadrant::BottomLeft:
		StartPoint.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X);
		StartPoint.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y) + HasEvenHeight;
		break;
	case EInventoryTileQuadrant::BottomRight:
		StartPoint.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X) + HasEvenWidth;
		StartPoint.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y) + HasEvenHeight;
		break;
	default:
		UE_LOG(LogSlimInventory ,Error , TEXT("Invalid Quadrant."));
		return FIntPoint( -1 , -1 );
	}

	return StartPoint;
}
FSlimSpaceQueryResult USlim_InventoryGrid::CheckHoverPosition(const FIntPoint& Position, const FIntPoint& Dimensions) const
{
	FSlimSpaceQueryResult Result;
	//check hover position
			// int the grid bounds?
			// any items in the way ?
			//if so , is there only one item in the way?( can we swap?)
	return Result;
}
#pragma endregion

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

#pragma region 绑定多播委托代理的函数
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

void USlim_InventoryGrid::AddStackNumer(const FSlimSlotAvailabilityResult& Result)
{
	//实现参数的更新
	if ( !MatchesCategory(Result.Item.Get()) ) return;

	//遍历可用性
	for ( const auto& Availability : Result.SlotAvailiabilites )
	{
		if ( Availability.bItemAtIndex )
		{
			const auto& GridSlot = GridSlots[Availability.SlotIndex];//获取网格插槽
			const auto& SlotItem = SlottedItems.FindChecked( Availability.SlotIndex );//获取插槽小部件

			SlotItem->UpdateStackCount( GridSlot->GetStackCount() + Availability.AmountToFill );
			GridSlot->SetStackCount( GridSlot->GetStackCount() +  Availability.AmountToFill );
		}
		else
		{
			AddItemAtIndex( Result.Item.Get() , Availability.SlotIndex , Result.bStackable , Availability.AmountToFill );//根据索引添加项目
			UpdateGridSlots( Result.Item.Get() , Availability.SlotIndex , Result.bStackable , Availability.AmountToFill );//更新网格插槽
		}
	}
}



#pragma endregion

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


