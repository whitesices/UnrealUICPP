// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/Slim_InventoryGrid.h"

#include "Widgets/Inventory/GridSlots/InventoryGridSlot.h"
#include "Widgets/Utils/InventoryWidgetUtils.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void USlim_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//调用初始化网格函数
	ConstructGrid();
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
