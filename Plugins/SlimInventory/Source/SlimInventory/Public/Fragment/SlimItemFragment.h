// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "SlimItemFragment.generated.h"

USTRUCT(BlueprintType)
struct FSlimItemFragment
{
	GENERATED_BODY()
public:
	//初始化构造函数
	FSlimItemFragment() {}
	FSlimItemFragment(const FSlimItemFragment&) = default;
	//重载运算符
	FSlimItemFragment& operator=(const FSlimItemFragment&) = default;
	FSlimItemFragment(FSlimItemFragment&&) = default;
	FSlimItemFragment& operator=(FSlimItemFragment&&) = default;
	//析构函数
	virtual ~FSlimItemFragment() {}

	//片段Tag添加
	FGameplayTag GetFragmentTag() const { return FragmentTag; }

private:
	//声明FragmentTag
	UPROPERTY( EditAnywhere , Category = "Inventory" )
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
};

//声明网格片段
USTRUCT(BlueprintType)
struct  FSlimGridFragment : public FSlimItemFragment
{
	GENERATED_BODY()
public:
	//设置尺寸 间隔 并获取相应的参数
	void SetGirdSize(const FIntPoint& Size) { GridSize = Size; }
	FIntPoint GetGridSize()  const { return GridSize; }

	void SetGridPadding(float& Padding) { GridPadding = Padding;  }
	float GetGridPadding() const { return GridPadding; }
private:
	//声明点位
	UPROPERTY( EditAnywhere , Category="Inventory")
	FIntPoint GridSize{ 1 , 1 };
	//声明与边缘的间隔
	UPROPERTY( EditAnywhere , Category="Inventory")
	float GridPadding{0.f};
};

//声明图片片段
USTRUCT(BlueprintType)
struct FSlimImageFragment : public FSlimItemFragment
{
	GENERATED_BODY()
public:
	//获取图标资源
	UTexture2D* GetIcon() const { return Icon; }
private:
	//设置Icon
	UPROPERTY( EditAnywhere , Category = "Inventory")
	TObjectPtr<UTexture2D> Icon{nullptr};
	//设置Icon的尺寸
	UPROPERTY( EditAnywhere , Category = "Inventory")
	FVector2D IconDimensions{ 44.f , 44.f };
};