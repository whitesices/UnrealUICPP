// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Components/SlimInventoryComponent.h"
//引入自定义的UI
#include "Widgets/Inventory/InventoryBase/Slim_InventoryBase.h"
#include "Items/Components/SlimInventoryItemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Items/SlimInventoryItem.h"
//引入自定义的片段
#include "Fragment/SlimItemFragment.h"

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

	//获取小部件是否有效
	USlimInventoryItem* FoundItem = InventoryList.FindFirstItemByType( ItemComponent->GetItemManifest().GetItemType() );
	Result.Item = FoundItem;

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
		OnStackChange.Broadcast(Result);//广播堆叠数量的变化
		Server_AddStacksToItem( ItemComponent , Result.TotalRoomToFill , Result.Remainder );
	}
	else if ( Result.TotalRoomToFill > 0.f )
	{
		//该条目不存在在库存UI上，创建一个新的并更新一个固定的插槽
		Server_AddNewItem( ItemComponent , Result.bStackable ? Result.TotalRoomToFill : 0 );
	}
}
#pragma region Server->Clienct Functions
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

	ItemComponent->Pickup();//调用Pickup函数来销毁对应的Item
}

void USlimInventoryComponent::Server_AddStacksToItem_Implementation( USlimInventoryItemComponent* ItemComponent, int32 StackCount, int32 Remainder )
{
	const FGameplayTag& ItemType = IsValid(ItemComponent) ? ItemComponent->GetItemManifest().GetItemType() : FGameplayTag::EmptyTag;
	USlimInventoryItem* Item = InventoryList.FindFirstItemByType( ItemType );

	if (!IsValid(Item))
	{
		return;
	}

	Item->SetTotalStackCount( Item->GetTotalStackCount() + StackCount );

	//TODO: Destroy the Item if the Remainder is zero
	//Otherwise , update the stack count fro the item pickup

	if ( Remainder == 0 )
	{
		ItemComponent->Pickup();//销毁对应的Item
	}
	else if( FSlimStackFragment* StackableFragment =  ItemComponent->GetItemManifest().GetFragmentOfTypeMutable<FSlimStackFragment>() )
	{
		StackableFragment->SetStackCount(Remainder);//更新堆叠数量
	}
	
}

void USlimInventoryComponent::Server_DropItem_Implementation(USlimInventoryItem* Item, int StackCount)
{
	/*SpawnDroppedItem(Item, StackCount);*/
	const int32 CurrentStackCount = Item->GetTotalStackCount() - StackCount;//获取当前的堆叠数
	//判断当前的堆叠数量是否有效
	if (CurrentStackCount <= 0)
	{
		InventoryList.RemoveEntry(Item);
	}
	else
	{
		Item->SetTotalStackCount(CurrentStackCount);//更新最大堆叠数
	}
	
	SpawnDroppedItem(Item,StackCount);
}

void USlimInventoryComponent::SpawnDroppedItem(USlimInventoryItem* Item, int32 StackCount)
{

	//TODO: 在场景中创建被丢弃的物品
	const APawn* OwningPawn = OwningPlayerController->GetPawn();//获取拥有的Pawn
	FVector RotatedForWard = OwningPawn->GetActorForwardVector();//获取Pawn的前向量
	RotatedForWard = RotatedForWard.RotateAngleAxis( FMath::FRandRange( DropSpawnAngleMin , DropSpawnAngleMax ), FVector::UpVector );
	FVector SpawnDropLocation = OwningPawn->GetActorLocation() + RotatedForWard * FMath::FRandRange( DropSpawnDistanceMin , DropSpawnDistanceMax );
	SpawnDropLocation.Z -= RelativeSpawnElevation;//调整Z轴的高度
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	//TODO Have the item manifest spawn the pickup actor
	FSlimItemManifest ItemManifest = Item->GetItemManifestMutable();//获取对应的物品清单
	//判断物品是否可以堆叠
	if (FSlimStackFragment* StackableFragment = ItemManifest.GetFragmentOfTypeMutable<FSlimStackFragment>() )
	{
		StackableFragment->SetStackCount(StackCount);//更新堆叠数量
	}

	ItemManifest.SpawnPickupActor( this , SpawnDropLocation , SpawnRotation );
}

void USlimInventoryComponent::Server_Consumeable_Implementation(USlimInventoryItem* Item, int StackCount)
{
	//更新当前的堆叠数量
	const int32 CurrentStackCount = Item->GetTotalStackCount() - 1;
	//判断当前堆叠数量是否有效
	if( CurrentStackCount <= 0 )
	{
		InventoryList.RemoveEntry(Item);
	}
	else
	{
		Item->SetTotalStackCount(CurrentStackCount);//更新最大堆叠数
	}

	//TODO: Get the consumeble fragment and call Consume
	//(Actually create the Consumeable Fragment )
	if (FSlimConsumeableFrgament* ConsumeableFragement = Item->GetItemManifestMutable().GetFragmentOfTypeMutable<FSlimConsumeableFrgament>())
	{
		ConsumeableFragement->OnConsumeable( OwningPlayerController.Get() );
	}
}

#pragma endregion

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

