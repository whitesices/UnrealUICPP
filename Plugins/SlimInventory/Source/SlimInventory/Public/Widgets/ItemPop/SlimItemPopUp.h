// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlimItemPopUp.generated.h"

//前置声明
//当鼠标右键点击物品时弹出菜单
class UButton;
class UTextBlock;
class USlider;
class USizeBox;

//建立委托
DECLARE_DYNAMIC_DELEGATE_TwoParams( FPopUpMenuSplit , int32 , SplitAmount , int32 , Index );
DECLARE_DYNAMIC_DELEGATE_OneParam( FPopUpMenuDrop , int32 , Index );
DECLARE_DYNAMIC_DELEGATE_OneParam( FPopUpMenuConsume , int32 , Index );

/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API USlimItemPopUp : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	//覆写鼠标离开事件
	virtual void NativeOnMouseLeave( const FPointerEvent& InMouseEvent ) override;

	//声明委托变量
	FPopUpMenuSplit MenuSplit;
	FPopUpMenuDrop MenuDrop;
	FPopUpMenuConsume MenuConsume;

	int32 GetSplitAmount() const;
	//声明折叠函数
	void collapseSplitButton() const;
	void collapseConsumeBUtton() const;
	void SetSliderParams( const float Max , const float Value ) const;

	FVector2D GetBoxSzie() const;
	void SetGridIndex(int32 Index) { GridIndex = Index; }
	int32 GetGridIndex() const { return GridIndex; }



private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Split;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Drop;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Consume;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USlider> Slider_Split;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_SplitAmount;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	//声明网格物品的索引
	int32 GridIndex{INDEX_NONE};

	//声明小组件绑定的触发函数
	UFUNCTION()
	void SplitButtonClicked();
	UFUNCTION()
	void DropButtonClicked();
	UFUNCTION()
	void ConsumeButtonClicked();

	UFUNCTION()
	void SliderValueChanged(float value);

	
};
