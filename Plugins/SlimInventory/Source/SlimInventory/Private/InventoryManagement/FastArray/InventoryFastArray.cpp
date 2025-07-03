// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/FastArray/InventoryFastArray.h"
#include "Items/SlimInventoryItem.h"
#include "Items/Components/SlimInventoryItemComponent.h"
#include "InventoryManagement/Components/SlimInventoryComponent.h"

TArray<USlimInventoryItem*> FSlimInventoryFastArray::GetAllItems() const
{
	//声明一个数组变量存储返回到结果
	TArray<USlimInventoryItem*> Result;

	Result.Reserve(Entries.Num());

	for ( const auto& Entry : Entries )
	{
		if (! IsValid(Entry.Item) )
		{
			continue;
		}
		Result.Add(Entry.Item);
	}
	return Result;
}

void FSlimInventoryFastArray::PreReplicatedRemove(const TArrayView<int32> RemoveIndices, int32 FinalSize)
{
	//声明InventoryComponent变量
	USlimInventoryComponent* InventoryComponent = Cast<USlimInventoryComponent>(OwnerComponent);
	//检查InventoryComponent是否有效
	if (!IsValid(InventoryComponent)) return;

	//遍历需要移除的索引
	for ( int32 Index : RemoveIndices )
	{
		InventoryComponent->OnItemRemoved.Broadcast(Entries[Index].Item);//广播相应的信息
	}
}

void FSlimInventoryFastArray::PostReplicatedAdd(const TArrayView<int32> AddIndices, int32 FinalSize)
{
	//声明InventoryComponent变量
	USlimInventoryComponent* InventoryComponent = Cast<USlimInventoryComponent> (OwnerComponent);
	if (!IsValid(InventoryComponent))
	{
		return;
	}

	//遍历需要添加到索引
	for ( int32 Index : AddIndices )
	{
		InventoryComponent->OnItemAdded.Broadcast(Entries[Index].Item);//广播相应的信息
	}
}

USlimInventoryItem* FSlimInventoryFastArray::AddEntry(USlimInventoryItemComponent* ItemComponent)
{
	//TODO : Imlement once ItemComponent is more complete
	//检查所属组件的有效性
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority() );

	//获取InventoryComponent
	USlimInventoryComponent* InventoryComponent = Cast<USlimInventoryComponent>(OwnerComponent);
	//检查InventtoryComponent的有效性
	if (!IsValid(InventoryComponent)) return nullptr;

	FSlimInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();

	NewEntry.Item = ItemComponent->GetItemManifest().Manifest(OwningActor);

	InventoryComponent->AddReplicatedSubObject( NewEntry.Item );
	MarkItemDirty(NewEntry);

	return NewEntry.Item;
}

USlimInventoryItem* FSlimInventoryFastArray::AddEntry(USlimInventoryItem* Item)
{
	//检查所属组件的有效性
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	//检查所属actor所有权
	check( OwningActor->HasAuthority());

	FSlimInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Item = Item;

	MarkItemDirty(NewEntry);
	return Item;
}

void FSlimInventoryFastArray::RemoveEntry(USlimInventoryItem* Item)
{
	//遍历移除
	for ( auto EntryIt = Entries.CreateIterator() ; EntryIt ; ++EntryIt )
	{
		FSlimInventoryEntry& Entry = *EntryIt;
		//判断Item
		if (Entry.Item == Item)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}
