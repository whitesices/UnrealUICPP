// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SlimInventoryItemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) , Blueprintable )
class SLIMINVENTORY_API USlimInventoryItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USlimInventoryItemComponent();

	//声明获取信息的方法
	FString GetPickupNessage() const
	{
		return PickupMessage;
	}
protected:


private:
	//声明传递的拾取信息
	UPROPERTY( EditAnywhere , Category = "Inventory" )
	FString PickupMessage;

		
};
