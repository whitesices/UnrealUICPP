// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/SlimInventoryItem.h"
//引入网络管理头文件
#include "Net/UnrealNetwork.h"


void USlimInventoryItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps( OutLifetimeProps );
	DOREPLIFETIME( ThisClass , ItemManiFest );
}

void USlimInventoryItem::SetItemManifest(const FSlimItemManifest& Manifest)
{
	//获取条目清单
	ItemManiFest = FInstancedStruct::Make<FSlimItemManifest>(Manifest);
}
