// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//引入自定义清单类头文件
#include "Items/Manifest/SlimItemManifest.h"
#include "SlimInventoryItemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) , Blueprintable )
class SLIMINVENTORY_API USlimInventoryItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USlimInventoryItemComponent();

	//复写获取生命周期内网络复制函数
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//定义初始化物品清单的方法
	void InitItemManifest( FSlimItemManifest CopyOfManifest );

	//获取清单信息
	FSlimItemManifest GetItemManifest() const
	{
		return ItemManifest;
	}

	//声明获取信息的方法
	FString GetPickupNessage() const
	{
		return PickupMessage;
	}
	//声明实现的Pickup函数
	void Pickup();
protected:

	//声明Pickup函数
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnPickUp();

private:
	//声明传递的拾取信息
	UPROPERTY( EditAnywhere , Category = "Inventory" )
	FString PickupMessage;
	//声明一个清单类变量
	UPROPERTY(Replicated , EditAnywhere , Category="Inventory")
	FSlimItemManifest ItemManifest;
		
};
