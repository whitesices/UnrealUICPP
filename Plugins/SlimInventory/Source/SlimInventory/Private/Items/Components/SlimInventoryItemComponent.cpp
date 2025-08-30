// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Components/SlimInventoryItemComponent.h"
//引入网络管理头文件
#include "Net/UnrealNetwork.h"

USlimInventoryItemComponent::USlimInventoryItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PickupMessage = FString("E - Pickup");
	SetIsReplicatedByDefault(true);//组件默认开始网络同步
}

void USlimInventoryItemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, ItemManifest);
}

void USlimInventoryItemComponent::InitItemManifest(FSlimItemManifest CopyOfManifest)
{
	ItemManifest = CopyOfManifest;
}

void USlimInventoryItemComponent::Pickup()
{
	OnPickUp();//调用蓝图实现的拾取函数
	GetOwner()->Destroy();//销毁
}




