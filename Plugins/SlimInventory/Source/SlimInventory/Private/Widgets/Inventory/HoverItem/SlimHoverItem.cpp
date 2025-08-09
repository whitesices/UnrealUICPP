// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/HoverItem/SlimHoverItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Items/SlimInventoryItem.h"

USlimInventoryItem* USlimHoverItem::GetInventoryItem() const
{
	return InventoryItem.Get();
}

void USlimHoverItem::SetInventoryItem(USlimInventoryItem* Item)
{
	InventoryItem = Item;
}

void USlimHoverItem::SetIsStackable(bool bStack)
{
	bIsStackable = bStack;
	if (!bStack)
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FGameplayTag USlimHoverItem::GetItemType() const
{
	if ( InventoryItem.IsValid() )
	{
		return InventoryItem->GetItemManifest().GetItemType();
	}

	return FGameplayTag();
}

void USlimHoverItem::SetImageBrush(const FSlateBrush& Brush) const
{
	Image_Icon->SetBrush(Brush);
}

void USlimHoverItem::UpdateStackCountText(const int32 Count) 
{
	StackCount = Count;
	if (Count > 0)
	{
		Text_StackCount->SetText( FText::AsNumber(Count) );
		
		Text_StackCount->SetVisibility( ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}
