#include "ExampleDebuggerCategory.h"
#include "CanvasItem.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

FExampleDebuggerCategory::FExampleDebuggerCategory()
{
	bShowOnlyWithDebugActor = false; // Line�� �� �����Ӹ��� �׸���
}

FExampleDebuggerCategory::~FExampleDebuggerCategory()
{
}

TSharedRef<FGameplayDebuggerCategory> FExampleDebuggerCategory::MakeInstance()
{
	return MakeShareable(new FExampleDebuggerCategory());
}

void FExampleDebuggerCategory::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	// Super�� Runtime ��忡���� ����ϹǷ� �θ� Ŭ������ Super�� ��ü
	FGameplayDebuggerCategory::CollectData(OwnerPC, DebugActor);

	//if (!!DebugActor)
	//	GLog->Log(DebugActor->GetName());

	ACharacter* player = OwnerPC->GetPawn<ACharacter>();

	// Player
	{
		PlayerPawnData.bDraw = true;
		PlayerPawnData.Name = player->GetName();
		PlayerPawnData.Location = player->GetActorLocation();
		PlayerPawnData.Forward = player->GetActorForwardVector();
	}

	//Forward Actor
	{
		FHitResult hitResult;

		FVector start = player->GetActorLocation();
		FVector end = start + player->GetActorForwardVector() * TraceDistance;

		FCollisionQueryParams params;
		params.AddIgnoredActor(player);

		player->GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, params);

		if (hitResult.bBlockingHit)
		{
			ForwardActorData.bDraw = true;
			ForwardActorData.Name = hitResult.GetActor()->GetName();
			ForwardActorData.Location = hitResult.GetActor()->GetActorLocation();
			ForwardActorData.Forward = hitResult.GetActor()->GetActorForwardVector();
		}
		else
		{
			ForwardActorData.bDraw = false;
		}
	}

	if(!!DebugActor)
	{
		SelectActorData.bDraw = true;
		SelectActorData.Name = DebugActor->GetName();
		SelectActorData.Location = DebugActor->GetActorLocation();
		SelectActorData.Forward = DebugActor->GetActorForwardVector();
	}
	else
	{
		SelectActorData.bDraw = false;
	}
}

void FExampleDebuggerCategory::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	// CanvasContext : Main UI ȭ��
	FGameplayDebuggerCategory::DrawData(OwnerPC, CanvasContext);

	FCanvasTileItem item(FVector2D(10, 10), FVector2D(300, 215), FLinearColor(0, 0, 0, 0.2f));
	item.BlendMode = ESimpleElementBlendMode::SE_BLEND_AlphaBlend; // �׷��� ���� �׸� ���� ����
	CanvasContext.DrawItem(item, CanvasContext.CursorX, CanvasContext.CursorY); // CanvasContext.CursorX : ĵ���� ���� ��ġ

	CanvasContext.Printf(FColor::Green, L"  -- Player Pawn --");
	CanvasContext.Printf(FColor::White, L"  Name : %s", *PlayerPawnData.Name);
	CanvasContext.Printf(FColor::White, L"  Location : %s", *PlayerPawnData.Location.ToString());
	CanvasContext.Printf(FColor::White, L"  Forward : %s", *PlayerPawnData.Forward.ToString());
	CanvasContext.Printf(FColor::White, L"");

	if(ForwardActorData.bDraw)
	{
		CanvasContext.Printf(FColor::Green, L"  -- Forward Actor --");
		CanvasContext.Printf(FColor::White, L"  Name : %s", *ForwardActorData.Name);
		CanvasContext.Printf(FColor::White, L"  Location : %s", *ForwardActorData.Location.ToString());
		CanvasContext.Printf(FColor::White, L"  Forward : %s", *ForwardActorData.Forward.ToString());
		CanvasContext.Printf(FColor::White, L"");
	}
	if(SelectActorData.bDraw)
	{
		CanvasContext.Printf(FColor::Green, L"  -- Select Actor --");
		CanvasContext.Printf(FColor::White, L"  Name : %s", *SelectActorData.Name);
		CanvasContext.Printf(FColor::White, L"  Location : %s", *SelectActorData.Location.ToString());
		CanvasContext.Printf(FColor::White, L"  Forward : %s", *SelectActorData.Forward.ToString());
	}

	FVector start = PlayerPawnData.Location;
	FVector end = PlayerPawnData.Location + PlayerPawnData.Forward * TraceDistance;

	DrawDebugLine(OwnerPC->GetWorld(), start, end, FColor::Red);
}

