// Fill out your copyright notice in the Description page of Project Settings.


#include "SlimInventoryPlayerController.h"
//引入插件类
#include "SlimInventory.h"
//引入增强输入组件
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Widgets/HUD/SlimInventoryWidget.h"
//引入GameplaySataic 
#include "Kismet/GameplayStatics.h"
//引入自定义的ItemComponent
#include "Items/Components/SlimInventoryItemComponent.h"
//引入自定义接口函数
#include "Interaction/Inventory_Hightlightable.h"
//引入自定义的inventoryComponent
#include "InventoryManagement/Components/SlimInventoryComponent.h"

ASlimInventoryPlayerController::ASlimInventoryPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	//初始化TraceLength
	TraceLenghth = 500.0f;
	//初始化碰撞类型
	ItemTraceChannel = ECC_GameTraceChannel1;
}

void ASlimInventoryPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//每帧调用TraceItem函数
	TraceItem();
}

void ASlimInventoryPlayerController::ToggleInventory()
{
	//判断Inventory是否有效
	if (!InventoryComp.IsValid()) return;
	//调用管理UI开关的函数
	InventoryComp->ToggleInventoryUI();
}

void ASlimInventoryPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogSlimInventory, Log , TEXT("Beginplay for SlimInventoryPlayerController") );

	//获取玩家的增强输入子系统
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( GetLocalPlayer() );
	if (IsValid(Subsystem))
	{//添加多个InputMappingContext
		for (UInputMappingContext* CurrentContext : DefaultIMCs)
		{
			Subsystem->AddMappingContext( CurrentContext , 0);
		}
	}

	//获取自定义的InventoryComponent
	InventoryComp = FindComponentByClass<USlimInventoryComponent>();

	//创建HUDWidget
	CreateHUDWidget();
}

void ASlimInventoryPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UE_LOG( LogSlimInventory , Log , TEXT("BeginPlay for PlayerController"));

	//获取玩家的增强输入组件
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	//绑定相应的输入
	EnhancedInputComponent->BindAction( PrimaryInteractAction , ETriggerEvent::Started , this , &ASlimInventoryPlayerController::PrimaryInetract);
	//绑定ToggleInventoryUI的InputAction
	EnhancedInputComponent->BindAction( ToggleInventoryUIInterAction , ETriggerEvent::Started , this , &ASlimInventoryPlayerController::ToggleInventory);

}

void ASlimInventoryPlayerController::PrimaryInetract()
{
	//打印相应的日志输出
	UE_LOG( LogSlimInventory , Log , TEXT("Primary interact") );
	//检查有效性
	if ( !ThisHitActor.IsValid() )
	{
		return;
	}

	USlimInventoryItemComponent* ItemComponent = ThisHitActor->FindComponentByClass<USlimInventoryItemComponent>();
	//检查ItemComponent 和 InventoryComponent
	if (!IsValid(ItemComponent) || !InventoryComp.IsValid())
	{
		return;
	}
	//调用添加条目
	InventoryComp->TryAddItem(ItemComponent);
}

void ASlimInventoryPlayerController::CreateHUDWidget()
{
	if (!IsLocalController())
	{
		return;
	}

	HUDWidget = CreateWidget<USlimInventoryWidget>( this , HUDWidgetClass );//创建UI进行UI变量初始化

	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();//添加到视口
	}
}

void ASlimInventoryPlayerController::TraceItem()
{
	//判断GEngine是否有效
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport))
	{
		UE_LOG(LogTemp , Error , TEXT("GEngine Is inValid"));
		return;
	}

	//声明视口尺寸
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	//声明视口中心
	const FVector2D ViewportCenter = ViewportSize / 2.f;
	//声明追踪的起终
	FVector TraceStart;
	FVector Forward;
	//判断是否超出屏幕
	if (!UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter, TraceStart, Forward))
	{
		return;
	}

	const FVector TraceEnd = TraceStart + Forward * TraceLenghth;
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel( HitResult , TraceStart , TraceEnd , ItemTraceChannel );

	//记录当前的HitActor
	LastHitActor = ThisHitActor;
	ThisHitActor = HitResult.GetActor();

	//判断当前的actor是否有效
	if (!ThisHitActor.IsValid())
	{
		if (IsValid(HUDWidget))
		{
			HUDWidget->HidePickupMessage();//隐藏相应的信息
		}
	}

	if (ThisHitActor == LastHitActor) return;

	if (ThisHitActor.IsValid())
	{
		//判断设置的静态组件是否有效
		if (UActorComponent* HighlightComp = ThisHitActor->FindComponentByInterface(UInventory_Hightlightable::StaticClass()))
		{
			IInventory_Hightlightable::Execute_Highlight(HighlightComp);//执行高亮显示
		}

		UE_LOG( LogTemp , Warning , TEXT("Started tracing a new actor"));
		//Execute showMessage
		USlimInventoryItemComponent* ItemComponent = ThisHitActor->FindComponentByClass<USlimInventoryItemComponent>();

		if (!IsValid(ItemComponent)) return;
		if (IsValid(HUDWidget))
		{
			HUDWidget->ShowPickupMessage(ItemComponent->GetPickupNessage());
		}
	}

	if ( LastHitActor.IsValid() )
	{
		UE_LOG( LogTemp , Warning , TEXT("Stopped tracing last actor"));
		//判断设置的静态组件是否有效
		if (UActorComponent* HighlightComp = LastHitActor->FindComponentByInterface(UInventory_Hightlightable::StaticClass()))
		{
			IInventory_Hightlightable::Execute_UnHightlight(HighlightComp);//执行高亮消失
		}
	}
}
