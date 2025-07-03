// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/SlimInfoMessage.h"
//引入文本组件头文件
#include "Components/TextBlock.h"

void USlimInfoMessage::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//初始文本内容为空且不显示消息
	Text_Message->SetText( FText::GetEmpty() );
	MessageHide();
}

void USlimInfoMessage::SetMessgage(const FText& Message)
{
	//设置文本内容
	Text_Message->SetText(Message);

	//判断记录文本显示的Flag
	if (!bIsMessageVisible)
	{
		MessageShow();
	}
	bIsMessageVisible = true;

	//定义计时器函数,定义一段时间后该文本消失
	GetWorld()->GetTimerManager().SetTimer(MessageTimerHandle, [this]()
	{
		MessageHide();
		bIsMessageVisible = false;
	},
	MessageDuration,
	false
	);


}
