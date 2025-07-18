// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Components/SlimInventoryComponent.h"
//引入自定义的UI
#include "Widgets/Inventory/InventoryBase/Slim_InventoryBase.h"
#include "Items/Components/SlimInventoryItemComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USlimInventoryComponent::USlimInventoryComponent():InventoryList(this)
{

	PrimaryComponentTick.bCanEverTick = false;
	//设置网络同步信息
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	bIsInventoryUIOpen = false;
}

void USlimInventoryComponent::ToggleInventoryUI()
{
	//通过定义的Flag来判断UI是否打开,执行bool值相反的操作
	if (bIsInventoryUIOpen)
	{
		CloseInventoryUI();
	}
	else
	{
		OpenInventoryUI();
	}
}

void USlimInventoryComponent::TryAddItem(USlimInventoryItemComponent* ItemComponent)
{
	

	//创建插槽
	FSlimSlotAvailabilityResult Result = InventoryUIMenu->HasRoomForItem(ItemComponent);

	if ( Result.TotalRoomToFill == 0 )
	{
		OnNoRoomInInventory.Broadcast();
		return;
	}
	//TODO:Actually add the item to inventory
	//判断结果是否有效
	if ( Result.Item.IsValid() && Result.bStackable )
	{
		//在已经存在的库存UI上按照栈的方式新增条目，只是更新栈的数量
		//不会创建新的条目
		Server_AddStacksToItem( ItemComponent , Result.TotalRoomToFill , Result.Remainder );
	}
	else if ( Result.TotalRoomToFill > 0.f )
	{
		//该条目不存在在库存UI上，创建一个新的并更新一个固定的插槽
		Server_AddNewItem( ItemComponent , Result.bStackable ? Result.TotalRoomToFill : 0 );
	}
}

//实现服务器发送新增条目的函数
void USlimInventoryComponent::Server_AddNewItem_Implementation( USlimInventoryItemComponent* ItemComponent, int32 StackCount )
{
	USlimInventoryItem* NewItem = InventoryList.AddEntry(ItemComponent);
	// TODO: Tell the Item Component to destroy its owning actor.
	//判断获取对象的有效性
	if ( GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone )
	{
		OnItemAdded.Broadcast(NewItem);
	}
}

void USlimInventoryComponent::Server_AddStacksToItem_Implementation( USlimInventoryItemComponent* ItemComponent, int32 StackCount, int32 Remainder )
{

}


void USlimInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME( ThisClass , InventoryList );
}

void USlimInventoryComponent::AddRepSubObject(UObject* Subobject)
{
	//判断是否被注册 是否准备好网络复制 对象是否有效
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(Subobject))
	{
		AddReplicatedSubObject(Subobject);
	}
}

void USlimInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ConstructInventory();
	
}

void USlimInventoryComponent::ConstructInventory()
{
	OwningPlayerController = Cast<APlayerController>( GetOwner() );
	checkf( OwningPlayerController.IsValid() , TEXT("Inventory Component should have a Player Controller as Owner"));//检查PlayerController是否有效

	if ( !OwningPlayerController->IsLocalController() )
	{
		return;
	}

	InventoryUIMenu = CreateWidget<USlim_InventoryBase>( OwningPlayerController.Get() , InventoryUIMenuClass );
	InventoryUIMenu->AddToViewport();
	//初始化关闭UI
	CloseInventoryUI();
}

void USlimInventoryComponent::OpenInventoryUI()
{
	//判断UI变量是否有效
	if (!IsValid(InventoryUIMenu))
	{
		UE_LOG( LogTemp , Warning , TEXT("Inventory is null") );
		return;
	}

	//设置UI可见性为可见
	InventoryUIMenu->SetVisibility( ESlateVisibility::Visible );
	//设置UI为打开状态
	bIsInventoryUIOpen = true;

	if (!OwningPlayerController.IsValid()) return;

	//设置输入模式
	FInputModeGameAndUI InputMode;
	OwningPlayerController->SetInputMode(InputMode);
	OwningPlayerController->SetShowMouseCursor(true);
}

void USlimInventoryComponent::CloseInventoryUI()
{
	//判断UI变量是否有效
	if (!IsValid(InventoryUIMenu))
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is null"));
		return;
	}
	//设置UI可见性为不可见
	InventoryUIMenu->SetVisibility( ESlateVisibility::Collapsed );
	//设置UI为关闭状态
	bIsInventoryUIOpen = false;

	if (!OwningPlayerController.IsValid()) return;

	//设置输入模式
	FInputModeGameAndUI InputMode;
	OwningPlayerController->SetInputMode(InputMode);
	OwningPlayerController->SetShowMouseCursor(false);
}

