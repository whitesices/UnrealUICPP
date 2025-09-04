// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/Slim_SpatialInventory.h"
//引入自定义的网格类
#include "Widgets/Inventory/Spatial/Slim_InventoryGrid.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Items/Components/SlimInventoryItemComponent.h"

#include "SlimInventory.h"
#include "InventoryManagement/Utils/SlimInventoryStatics.h"
#include "Components/CanvasPanel.h"
#include "Widgets/ItemDescription/SlimItemDescription.h"//引入ItemDescription头文件

void USlim_SpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//按钮事件绑定
	Button_Equippables->OnClicked.AddDynamic( this , &ThisClass::ShowEquippables);
	Button_Consumables->OnClicked.AddDynamic( this , &ThisClass::ShowConsumables);
	Button_Craftables->OnClicked.AddDynamic(this , &ThisClass::ShowCraftables);

	//声明各个网格的绑定
	Grid_Equippables->SetOwningCanvasPanel(CanvasPanel);
	Grid_Consumables->SetOwningCanvasPanel(CanvasPanel);
	Grid_Craftables->SetOwningCanvasPanel(CanvasPanel);

	ShowEquippables();
}

FReply USlim_SpatialInventory::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	ActivateGrid->DropItem();
	return FReply::Handled();
}

FSlimSlotAvailabilityResult USlim_SpatialInventory::HasRoomForItem(USlimInventoryItemComponent* ItemComponent) const
{
	/*return FSlimSlotAvailabilityResult();*/
	//FSlimSlotAvailabilityResult Result;
	//Result.TotalRoomToFill = 1;
	//return Result;
	switch ( USlimInventoryStatics::GetItemCategoryFromItemComp(ItemComponent) )
	{
		case EInventory_ItemCategory::Equippable:
			return Grid_Equippables->HasRoomForItem(ItemComponent);
		case EInventory_ItemCategory::Consumable:
			return Grid_Consumables->HasRoomForItem(ItemComponent);
		case EInventory_ItemCategory::Craftable:
			return Grid_Craftables->HasRoomForItem(ItemComponent);
		default:
			UE_LOG( LogSlimInventory , Error , TEXT("Itemcomponent doesn`t have a valid Item Category") );
			return FSlimSlotAvailabilityResult();
	}
}

#pragma region 重载父类Hover方法
void USlim_SpatialInventory::OnItemHovered(USlimInventoryItem* Item)
{
	Super::OnItemHovered(Item);
	USlimItemDescription* ItemDescriptionUI = GetItemDescription();
	ItemDescriptionUI->SetVisibility(ESlateVisibility::Collapsed);//设置为折叠
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);//先清除TimerHandle

	//声明Timer委托
	FTimerDelegate DescriptionTimerDelegate;
	DescriptionTimerDelegate.BindLambda([this]()
	{
		GetItemDescription()->SetVisibility(ESlateVisibility::HitTestInvisible);//设置显示但不遮挡
	}
	);

	GetOwningPlayer()->GetWorldTimerManager().SetTimer(DescriptionTimer, DescriptionTimerDelay, false);
}

void USlim_SpatialInventory::OnItemUnhovered()
{
	Super::OnItemUnhovered();
}

bool USlim_SpatialInventory::HasHoverItem() const
{
	if (Grid_Equippables->HasHoverItem()) return true;
	if (Grid_Consumables->HasHoverItem()) return true;
	if (Grid_Craftables->HasHoverItem()) return true;

	return false;
}
#pragma endregion

void USlim_SpatialInventory::ShowEquippables()
{
	//绑定相应的状态显示
	SetActivateGrid( Grid_Equippables , Button_Equippables );
}

void USlim_SpatialInventory::ShowConsumables()
{
	//绑定相应的状态显示
	SetActivateGrid(Grid_Consumables, Button_Consumables);
}

void USlim_SpatialInventory::ShowCraftables()
{
	//绑定相应的状态显示
	SetActivateGrid(Grid_Craftables, Button_Craftables);
}

void USlim_SpatialInventory::ResetButtonStates(UButton* Button)
{
	Button_Equippables->SetIsEnabled(true);
	Button_Consumables->SetIsEnabled(true);
	Button_Craftables->SetIsEnabled(true);
	Button->SetIsEnabled(false);

}

void USlim_SpatialInventory::SetActivateGrid(USlim_InventoryGrid* Grid, UButton* Button)
{
	if (ActivateGrid.IsValid()) ActivateGrid->HideTheCursor();
	ActivateGrid = Grid;
	if (ActivateGrid.IsValid()) ActivateGrid->ShowTheCursor();

	ResetButtonStates(Button);
	Switcher->SetActiveWidget(Grid);
}

#pragma region ItemDescription
USlimItemDescription* USlim_SpatialInventory::GetItemDescription()
{
	return nullptr;
}
#pragma endregion
