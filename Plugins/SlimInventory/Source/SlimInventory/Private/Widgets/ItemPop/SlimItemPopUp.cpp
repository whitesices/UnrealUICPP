// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ItemPop/SlimItemPopUp.h"
//#include "SlimItemPopUp.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

void USlimItemPopUp::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//绑定按钮点击事件
	Button_Split->OnClicked.AddDynamic(this , &ThisClass::SplitButtonClicked);
	Button_Drop->OnClicked.AddDynamic(this , &ThisClass::DropButtonClicked);
	Button_Consume->OnClicked.AddDynamic(this , &ThisClass::ConsumeButtonClicked);

	Slider_Split->OnValueChanged.AddDynamic(this , &ThisClass::SliderValueChanged);
}

void USlimItemPopUp::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	RemoveFromParent();//鼠标离开时移除该UI
}

int32 USlimItemPopUp::GetSplitAmount() const
{
	return FMath::Floor(Slider_Split->GetValue());//向下取整
}


void USlimItemPopUp::SplitButtonClicked()
{
	if ( MenuSplit.ExecuteIfBound(GetSplitAmount() , GridIndex) )
	{
		RemoveFromParent();
	}
}

void USlimItemPopUp::DropButtonClicked()
{
	if (MenuDrop.ExecuteIfBound(GridIndex))
	{
		RemoveFromParent();
	}
}

void USlimItemPopUp::ConsumeButtonClicked()
{
	if (MenuConsume.ExecuteIfBound(GridIndex))
	{
		RemoveFromParent();
	}
}

void USlimItemPopUp::SliderValueChanged(float value)
{
	Text_SplitAmount->SetText(FText::AsNumber(FMath::Floor(value)) );//更新文本数据
}

void USlimItemPopUp::collapseSplitButton() const
{
	//对UI组件进行折叠
	Button_Split->SetVisibility( ESlateVisibility::Collapsed);
	Slider_Split->SetVisibility( ESlateVisibility::Collapsed);
	Text_SplitAmount->SetVisibility(ESlateVisibility::Collapsed);
}

void USlimItemPopUp::collapseConsumeBUtton() const
{
	//折叠消耗品相关的UI组件
	Button_Consume->SetVisibility(ESlateVisibility::Collapsed);
}

void USlimItemPopUp::SetSliderParams(const float Max, const float Value) const
{
	//设置滑动条参数
	Slider_Split->SetMaxValue(Max);
	Slider_Split->SetMinValue(1);
	Slider_Split->SetValue(Value);
	Text_SplitAmount->SetText( FText::AsNumber(FMath::Floor(Value)) );
}

FVector2D USlimItemPopUp::GetBoxSzie() const
{
	return FVector2D( SizeBox_Root->GetWidthOverride() , SizeBox_Root->GetHeightOverride() );//返回网格尺寸大小
}
