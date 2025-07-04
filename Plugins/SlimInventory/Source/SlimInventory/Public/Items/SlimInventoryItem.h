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


private:
	//声明实例结构体
	UPROPERTY( VisibleAnywhere , meta = ( BaseStruct = "/Script/SlimInventory.SlimItemManifest" ) , Replicated )
	FInstancedStruct  ItemManiFest;
};
