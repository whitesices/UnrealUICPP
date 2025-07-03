// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlimInfoMessage.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API USlimInfoMessage : public UUserWidget
{
	GENERATED_BODY()
public:
	//覆写初始化函数
	virtual void NativeOnInitialized() override;
	//声明文本显示函数,在蓝图中实现
	UFUNCTION(BlueprintImplementableEvent , Category="Inventory")
	void MessageShow();
	//声明文本隐藏函数，在蓝图中实现
	UFUNCTION(BlueprintImplementableEvent , Category="Inventory")
	void MessageHide();

	//设置文本内容
	void SetMessgage( const FText& Message );

private:
	//设置在蓝图UI中需要绑定的相应文本组件
	UPROPERTY( meta=(BindWidget) )
	TObjectPtr<UTextBlock> Text_Message;
	//设置信息存在时间
	UPROPERTY( EditAnywhere , Category="Inventory" )
	float MessageDuration = 3.f;

	//声明一个计时器句柄
	FTimerHandle MessageTimerHandle;
	//声明一个Flag来判断信息是否显示
	bool bIsMessageVisible = false;
};
