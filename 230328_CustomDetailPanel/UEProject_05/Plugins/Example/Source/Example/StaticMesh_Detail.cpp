#include "StaticMesh_Detail.h"
#include "CStaticMesh.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"

TSharedRef<IDetailCustomization> FStaticMesh_Detail::MakeInstance()
{
	return MakeShareable(new FStaticMesh_Detail());
}

void FStaticMesh_Detail::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& mesh = DetailBuilder.EditCategory("Mesh");

	mesh.AddCustomRow(FText::FromString("Color")) // �˻� ����
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Color"))
	]
	.ValueContent()
	.VAlign(VAlign_Center)		// ��� ����
	.MaxDesiredWidth(250)		// �ִ� ����
	[
		SNew(SButton)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)	// �� ä�� ���·� ����
		.OnClicked(this, &FStaticMesh_Detail::OnClicked_Paint)
		.Content()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Paint"))
		]
	];

	// Ŭ���� Object�� List�� ��ȯ
	DetailBuilder.GetObjectsBeingCustomized(Objects);
}

FReply FStaticMesh_Detail::OnClicked_Paint()
{
	for(TWeakObjectPtr<UObject> obj : Objects)
	{
		ACStaticMesh* meshActor = Cast<ACStaticMesh>(obj);

		if (!!meshActor)
			meshActor->Paint();
	}

	return FReply::Handled();
}
