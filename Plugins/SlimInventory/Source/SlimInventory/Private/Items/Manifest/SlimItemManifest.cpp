// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Manifest/SlimItemManifest.h"
#include "Items/SlimInventoryItem.h"
#include "Items/Components/SlimInventoryItemComponent.h"//引入自定义的Item组件

USlimInventoryItem* FSlimItemManifest::Manifest(UObject* NewOuter)
{
    //声明库存条目变量
    USlimInventoryItem* NewItem = NewObject<USlimInventoryItem>(NewOuter , USlimInventoryItem::StaticClass() );
    NewItem->SetItemManifest(*this);

    return NewItem;
}

void FSlimItemManifest::AssimilateIventoryFragments(UUSlimCompositeBase* Composite) const
{
}

void FSlimItemManifest::SpawnPickupActor(const UObject* WorldContextObject, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
    //判断拾取的物品是否有效
    if (!IsValid(PickupActorClass) || !IsValid(WorldContextObject)) return;
    //缓存生成的物品
    AActor* CachedSpawnedActor = WorldContextObject->GetWorld()->SpawnActor<AActor>(PickupActorClass , SpawnLocation , SpawnRotation);
    //判断生成的物品是否有效
    if (!IsValid(CachedSpawnedActor)) return;
    //set the item manifest , item category , item type , etc
    USlimInventoryItemComponent* ItemComponent = CachedSpawnedActor->FindComponentByClass<USlimInventoryItemComponent>();//获取物品组件
    if(IsValid(ItemComponent))//若物品组件有效
    {
        ItemComponent->InitItemManifest(*this);//初始化物品清单
    }
}
