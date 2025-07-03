// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Manifest/SlimItemManifest.h"
#include "Items/SlimInventoryItem.h"

USlimInventoryItem* FSlimItemManifest::Manifest(UObject* NewOuter)
{
    //声明库存条目变量
    USlimInventoryItem* NewItem = NewObject<USlimInventoryItem>(NewOuter , USlimInventoryItem::StaticClass() );
    NewItem->SetItemManifest(*this);

    return NewItem;
}
