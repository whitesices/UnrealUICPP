// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Items/Manifest/SlimItemManifest.h"

#include "SlimInventoryItem.generated.h"

//struct FSlimItemManifest;

//引入自定义的清单类
//struct FSlimItemManifest;
//struct FInstancedStruct;
/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API USlimInventoryItem : public UObject
{
	GENERATED_BODY()

public:
	//复写获取生命周期内网络复制函数
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	//设置清单条目
	void SetItemManifest( const FSlimItemManifest& Manifest );

	//定义获取ManiFest的函数
	const FSlimItemManifest& GetItemManifest() const
	{
		return ItemManiFest.Get<FSlimItemManifest>();
	}

	FSlimItemManifest& GetItemManifestMutable()
	{
		return ItemManiFest.GetMutable<FSlimItemManifest>();
	}

	//定义一个判断是否可以堆叠的方法
	bool IsStackable() const;

	//定义和获取堆叠总数相关的函数
	int32 GetTotalStackCount() const { return TotalStackCount; }
	void SetTotalStackCount(int32 Count) { TotalStackCount = Count; }

private:
	//声明实例结构体
	UPROPERTY( VisibleAnywhere , meta = ( BaseStruct = "/Script/SlimInventory.SlimItemManifest" ) , Replicated )
	FInstancedStruct  ItemManiFest;

	//声明堆叠总数
	UPROPERTY(Replicated)
	int32 TotalStackCount = 1;
};

//声明模板函数获取对应的FragmentType
template<typename FragmentType>
const FragmentType* GetFragment( const USlimInventoryItem* Item , const FGameplayTag& Tag )
{
	//判断Item是否有效
	if (!IsValid(Item)) return nullptr;

	const FSlimItemManifest& Manifest = Item->GetItemManifest();
	return Manifest.GetFragmentOfTypeWithTag<FragmentType>(Tag);
}