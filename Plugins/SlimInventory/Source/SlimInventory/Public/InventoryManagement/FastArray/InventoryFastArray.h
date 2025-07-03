// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//引入FastArraySerialization头文件
#include "Net/Serialization/FastArraySerializer.h"

//添加反射文件
#include "InventoryFastArray.generated.h"




class USlimInventoryComponent;
class USlimInventoryItem;
class USlimInventoryItemComponent;

USTRUCT(BlueprintType)
struct FSlimInventoryEntry:public FFastArraySerializerItem
{
	GENERATED_BODY()
	//声明构造函数
public:
	FSlimInventoryEntry() {}
private:
	//声明朋友类
	friend USlimInventoryComponent;
	friend struct FSlimInventoryFastArray;

	//声明Item属性
	UPROPERTY()
	TObjectPtr<USlimInventoryItem> Item = nullptr;
};

USTRUCT(BlueprintType)
struct FSlimInventoryFastArray : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	//声明构造函数
	FSlimInventoryFastArray() : OwnerComponent(nullptr) {}//默认无参数构造函数
	FSlimInventoryFastArray(UActorComponent* InOwnerComponent) :OwnerComponent(InOwnerComponent) {}//有参数构造函数
	//定义获取所有Item的函数
	TArray<USlimInventoryItem*> GetAllItems() const;

	//begin FFastArraySerializer contract
	void PreReplicatedRemove( const TArrayView<int32> RemoveIndices , int32 FinalSize ) ;
	void PostReplicatedAdd( const TArrayView<int32> AddIndices , int32 FinalSize );
	//end FFastArraySerializer contract

	//定义网络序列化函数
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams )
	{
		return FastArrayDeltaSerialize<FSlimInventoryEntry , FSlimInventoryFastArray >( Entries , DeltaParams , *this );
	}

	//定义添加相应条目的函数
	USlimInventoryItem* AddEntry( USlimInventoryItemComponent*  ItemComponent);
	USlimInventoryItem* AddEntry( USlimInventoryItem* Item);
	//定义移除相应条目的函数
	void RemoveEntry(USlimInventoryItem* Item);

private:
	//声明相应的参数属性
	//声明朋友类
	friend USlimInventoryComponent;

	//Item的网络复制
	UPROPERTY()
	TArray<FSlimInventoryEntry> Entries;
	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

//声明模板类
template<>
struct TStructOpsTypeTraits<FSlimInventoryFastArray> :public TStructOpsTypeTraitsBase2<FSlimInventoryFastArray>
{
	enum { WithNetDeltaSerializer = true };
};
