// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Items/Manifest/SlimItemManifest.h"

#include "SlimInventoryItem.generated.h"

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

	//设置清单条目
	void SetItemManifest( const FSlimItemManifest& Manifest );

private:
	//声明实例结构体
	UPROPERTY( VisibleAnywhere , meta = ( BaseStruct = "/Script/SlimInventory.SlimItemManifest" ) , Replicated )
	FInstancedStruct  ItemManiFest;
};
