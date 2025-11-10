// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Composite/USlimCompositeBase.h"
#include "USlimComposite.generated.h"

/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API UUSlimComposite : public UUSlimCompositeBase
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;//复写本地初始化函数
	virtual void ApplyFunction(FuncType Function) override;//复写应用函数
	virtual void Collapse() override;//复写折叠函数


private:
	UPROPERTY()
	TArray< TObjectPtr<UUSlimCompositeBase> > Children;//定义孩子组件数组
	
};
