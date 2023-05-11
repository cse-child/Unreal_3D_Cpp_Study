#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Combo.generated.h"

UCLASS(Blueprintable)
class UEPROJECT_06_API UCDoAction_Combo : public UCDoAction
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

public:
	void OnAttachmentEndCollision() override;

	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther) override;

private:
	int32 Index;

	bool bEnable;	// 입력 허용 상태인가
	bool bExist;	// 입력이 들어온 상태인가

private:
	TArray<class ACharacter*> Hitted; // Hitted 된 캐릭터 배열 (중복충돌X)
};