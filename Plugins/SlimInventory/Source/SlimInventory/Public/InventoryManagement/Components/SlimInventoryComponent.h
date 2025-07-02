// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SlimInventoryComponent.generated.h"

class USlim_InventoryBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) , Blueprintable )
class SLIMINVENTORY_API USlimInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USlimInventoryComponent();

	//声明触发打开InventoryUI的函数
	void ToggleInventoryUI();

protected:
	virtual void BeginPlay() override;

private:	
	//声明PlayerController变量
	TWeakObjectPtr<APlayerController> OwningPlayerController;

	//建立新的初始化函数
	void ConstructInventory();

	//声明自定义的UI基类变量
	UPROPERTY()
	TObjectPtr<USlim_InventoryBase> InventoryUIMenu;

	//声明一个变量来存储UI的类
	UPROPERTY( EditAnywhere , Category = "Inventory" )
	TSubclassOf<USlim_InventoryBase> InventoryUIMenuClass;

	//声明flag来存储UI是否打开
	bool bIsInventoryUIOpen;
	//声明打开UI的函数
	void OpenInventoryUI();
	//声明关闭UI的函数
	void CloseInventoryUI();
		
};
