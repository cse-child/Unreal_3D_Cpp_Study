#include "Characters/CEnemy_AI.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Label.h"


ACEnemy_AI::ACEnemy_AI()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UWidgetComponent>(this, &LabelWidget, "Label", GetMesh());

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	CHelpers::CreateActorComponent<UCAIBehaviorComponent>(this, &Behavior, "Behavior");

	TSubclassOf<UCUserWidget_Label> labelClass;
	CHelpers::GetClass<UCUserWidget_Label>(&labelClass, "WidgetBlueprint'/Game/Widgets/WB_Label.WB_Label_C'");
	LabelWidget->SetWidgetClass(labelClass);
	LabelWidget->SetRelativeLocation(FVector(0, 0, 220));
	LabelWidget->SetDrawSize(FVector2D(120, 0));
	LabelWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ACEnemy_AI::BeginPlay()
{
	Super::BeginPlay();

	LabelWidget->InitWidget();
	UCUserWidget_Label* label = Cast<UCUserWidget_Label>(LabelWidget->GetUserWidgetObject());
	label->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
	label->UpdateName(GetName());
	label->UpdateControllerName(GetController()->GetName());
}

void ACEnemy_AI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UCUserWidget_Label* label = Cast<UCUserWidget_Label>(LabelWidget->GetUserWidgetObject());
	if(!!label)
	{
		label->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
	}
}
