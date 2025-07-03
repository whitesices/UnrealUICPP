// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/SlimInventoryWidget.h"
#include "Widgets/HUD/SlimInfoMessage.h"
#include "InventoryManagement/Components/SlimInventoryComponent.h"
#include "InventoryManagement/Utils/SlimInventoryStatics.h"

void USlimInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//声明InventoryConponent变量
	USlimInventoryComponent* InventoryComponent = USlimInventoryStatics::GetInventoryComponent( GetOwningPlayer() );//通过自定义的蓝图库函数获取InventoryComponent
	//检查组件的有效性
	if (IsValid(InventoryComponent))
	{
		InventoryComponent->OnNoRoomInInventory.AddDynamic( this , &USlimInventoryWidget::NoRoom);
	}
}

void USlimInventoryWidget::NoRoom()
{
	//检查消息组件的有效性
	if (!IsValid(InfoMessage)) return;
	InfoMessage->SetMessgage( FText::FromString("No room In Inventory") );
}
