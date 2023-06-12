#pragma once

#include "CoreMinimal.h"


class WEAPON_API SWeaponCheckBoxes : public TSharedFromThis<SWeaponCheckBoxes>
{
public:
	void AddProperties(TSharedPtr<IPropertyHandle> InHandle);

	void SetUtilities(TSharedPtr<class IPropertyUtilities> InUtilities);
	
	TSharedRef<class SWidget> Draw(bool bBackground = false); // SWidget을 생성하고 바로 반환해서 사용해주려면 TSharedRef 를 사용해야함
	void DrawProperties(TSharedRef<IPropertyHandle> InPropertyHandle, class IDetailChildrenBuilder* InChildBuilder);

private:
	TSharedRef<SWidget> DrawCheckBox(int32 InIndex);

public:
	void CheckDefaultObject(int32 InIndex, UObject* InValue); //UObject
	void CheckDefaultValue(int32 InIndex, float InValue);
	void CheckDefaultValue(int32 InIndex, bool InValue);
	void CheckDefaultValue(int32 InIndex, const FVector& InValue);

private:
	void OnCheckStateChanged(ECheckBoxState InState, int InIndex);

public:
	static bool CanDraw(TSharedRef<IPropertyHandle> InHandle, int InCount);

private:
	TSharedPtr<class IPropertyUtilities> Utilities;

public:
	struct FInternalData // 내부에서만 사용하기 위한 구조체
	{
		bool bChecked;
		FString Name;
		TSharedPtr<IPropertyHandle> Handle; // CustomimzeChildren 함수의 항목 한줄한줄의 Handle을 받음

		FInternalData(TSharedPtr<IPropertyHandle> InHandle)
		{
			bChecked = false;
			Handle = InHandle;

			/* Name 목록 : Montage / Play Rate / Can Move / Use Control Rotation */
			Name = Handle->GetPropertyDisplayName().ToString(); 
		}
	};

	TArray<FInternalData> InternalDatas;
};


