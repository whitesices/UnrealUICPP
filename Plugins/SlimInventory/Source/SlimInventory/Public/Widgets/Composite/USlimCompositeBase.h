// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "USlimCompositeBase.generated.h"

/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API UUSlimCompositeBase : public UUserWidget
{
	GENERATED_BODY()
public:
	FGameplayTag GetFragmentTag() const { return FragmentTag; }//获取片段游戏标签
	void SetFragmentTag(const FGameplayTag& CurrentTag) { FragmentTag = CurrentTag; }//设置片段游戏标签
	virtual void Collapse();//定义折叠函数
	void Expand();//定义展开函数

	using FuncType = TFunction<void(UUSlimCompositeBase*)>;//定义TFunction
	virtual void ApplyFunction(FuncType Function) {}//定义应用函数

private:
	//声明片段游戏标签
	UPROPERTY( EditAnywhere , Category = "Inventory")
	FGameplayTag FragmentTag;
	
};
