// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SlimInventoryPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class USlimInventoryWidget;
/**
 * 
 */
UCLASS()
class SLIMINVENTORY_API ASlimInventoryPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ASlimInventoryPlayerController();
	//重载Tick函数
	virtual void Tick(float DeltaSeconds) override;
protected:
	//重载Beginplay
	virtual void BeginPlay() override;
	//重载设置输入组件
	virtual void SetupInputComponent() override;

private:
	//自定义主要输入方法
	void PrimaryInetract();
	//自定义创建UI的方法
	void CreateHUDWidget();
	//自定义创建TraceItem的方法
	void TraceItem();

	//定义InputMapContext
	UPROPERTY(EditDefaultsOnly , Category = "Inventory")
	TArray< TObjectPtr<UInputMappingContext> > DefaultIMCs;
	//定义InputAction
	UPROPERTY(EditDefaultsOnly , Category = "Inventory")
	TObjectPtr<UInputAction> PrimaryInteractAction;
	//声明HUDWidgetClass
	UPROPERTY(EditDefaultsOnly , Category = "Inventory")
	TSubclassOf<USlimInventoryWidget> HUDWidgetClass;
	//声明HUDWidget
	UPROPERTY(VisibleAnywhere , Category= "Inventory")
	TObjectPtr<USlimInventoryWidget> HUDWidget;

	//定义TraceLength
	UPROPERTY(EditDefaultsOnly , Category = "Inventory")
	double TraceLenghth;

	//声明Trace类型
	UPROPERTY( EditDefaultsOnly , Category = "Inventory")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;

	//声明记录Trace命中的前后对象
	TWeakObjectPtr<AActor> ThisHitActor;
	TWeakObjectPtr<AActor> LastHitActor;
};
