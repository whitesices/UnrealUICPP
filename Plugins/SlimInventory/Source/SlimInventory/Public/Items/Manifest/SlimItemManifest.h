// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/InventoryGridTypes.h"
#include "StructUtils/InstancedStruct.h"
//引入gameplayTag
#include "GameplayTagContainer.h"

#include "SlimItemManifest.generated.h"
/**
 * 
 */

class USlimInventoryItem;
struct FSlimItemFragment;

USTRUCT(BlueprintType)
struct SLIMINVENTORY_API FSlimItemManifest
{
	GENERATED_BODY()

	USlimInventoryItem* Manifest( UObject* NewOuter);

	EInventory_ItemCategory GetItemCatgory() const
	{
		return ItemCategory;
	}

	//返回物品的标签
	FGameplayTag GetItemType() const
	{
		return ItemType;
	}

	//声明一个模板函数 去存储不同类型
	template<typename T> requires std::derived_from< T , FSlimItemFragment >
	const T* GetFragmentOfTypeWithTag( const FGameplayTag& FragmentTag ) const;

	//声明一个模板函数去获取对应片段的类型
	template<typename T> requires std::derived_from< T , FSlimItemFragment >
	const T* GetFragmentOfType() const;

	//声明一个模板类函数去通过类型匹配获取对应的片段
	template<typename T> requires std::derived_from< T , FSlimItemFragment>
	T* GetFragmentOfTypeMutable();

private:
	//定义选择的属性
	UPROPERTY( EditAnywhere , Category = "Inventory" )
	EInventory_ItemCategory ItemCategory{ EInventory_ItemCategory::None };

	//定义物品的标签
	UPROPERTY( EditAnywhere , Category = "Inventory" )
	FGameplayTag ItemType;

	//声明小部件片段
	UPROPERTY( EditAnywhere , Category="Inventory" , meta=(ExcludeBaseStruct) )
	TArray< TInstancedStruct<FSlimItemFragment> > Fragments;
};

template<typename T> requires std::derived_from< T, FSlimItemFragment >
inline const T* FSlimItemManifest::GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const
{
	//遍历查询对应的Tag
	for ( const TInstancedStruct<FSlimItemFragment>& Fragment : Fragments )
	{
		if ( const T* FragmentPtr = Fragment.GetPtr<T>() )
		{
			if (!FragmentPtr->GetFragmentTag().MatchesTagExact(FragmentTag))
			{
				continue;
			}

			return FragmentPtr;
		}
	 }

	return nullptr;
}

template<typename T> requires std::derived_from< T, FSlimItemFragment >
inline const T* FSlimItemManifest::GetFragmentOfType() const
{
	for ( const TInstancedStruct<FSlimItemFragment>& Fragment : Fragments )
	{
		if ( const T* FragmentPtr = Fragment.GetPtr<T>() )
		{
			return FragmentPtr;
		}
	}
	return nullptr;
}

template<typename T> requires std::derived_from< T, FSlimItemFragment >
inline T* FSlimItemManifest::GetFragmentOfTypeMutable()
{
	for ( TInstancedStruct<FSlimItemFragment>& Fragment : Fragments )
	{
		if ( T* FragmentPtr = Fragment.GetMutablePtr<T>() )
		{
			return FragmentPtr;//返回对应的片段
		}
	}
	return nullptr;
}
