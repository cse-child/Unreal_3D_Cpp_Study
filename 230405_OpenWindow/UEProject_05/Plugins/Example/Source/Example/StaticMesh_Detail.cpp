#include "StaticMesh_Detail.h"
#include "CStaticMesh.h"
#include "DesktopPlatformModule.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDesktopPlatform.h"
#include "Engine/StaticMeshSocket.h"
#include "Interfaces/IMainFrameModule.h"
#include "Misc/FileHelper.h"
#include "Misc/MessageDialog.h"
#include "Serialization/BufferArchive.h"

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

	mesh.AddCustomRow(FText::FromString("SaveMesh")) // �˻� ����
		.NameContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Mesh"))
		]
	.ValueContent()
		.VAlign(VAlign_Center)		// ��� ����
		.MaxDesiredWidth(250)		// �ִ� ����
		[
			SNew(SButton)
			.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)	// �� ä�� ���·� ����
		.OnClicked(this, &FStaticMesh_Detail::OnClicked_Save)
		.Content()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Save"))
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

FReply FStaticMesh_Detail::OnClicked_Save()
{
	if(Objects[0]->GetWorld()->IsPlayInEditor())
	{
		FMessageDialog dialog;
		dialog.Debugf(FText::FromString("In Game mode not working"));

		return FReply::Handled();
	}

	// ������ Mesh ���� meshActors�� �����ϱ�
	TArray<ACStaticMesh*> meshActors;
	for(TWeakObjectPtr<UObject> obj : Objects)
	{
		ACStaticMesh* meshActor = Cast<ACStaticMesh>(obj);
		if (meshActor == nullptr) continue;

		meshActors.Add(meshActor);
	}

	/* Message Dialog �׽�Ʈ */
	//{
	//	FText title = FText::FromString("Warning");
	//	FText context = FText::FromString(FString::Printf(L"Selected Num : %d", meshActors.Num()));
	//	FMessageDialog::Debugf(context, &title);
	//}

	// ���õ� Mesh���� Save ��� ����
	for(int32 i = 0; i < meshActors.Num(); i++)
	{
		UActorComponent* actorComponent = meshActors[i]->GetComponentByClass(UStaticMeshComponent::StaticClass());
		UStaticMeshComponent* meshComponent = Cast<UStaticMeshComponent>(actorComponent);

		// mesh Component�� �������� �ʾƼ� ó���� ��ư ����� ���ٸ� ���� ���̾�� Pass
		if (meshComponent == nullptr) FReply::Unhandled();

		// ������ ȭ�鿡 �׷����� ���� Mesh ������Ʈ �ȿ��ִ� Static Mesh
		UStaticMesh* mesh = meshComponent->GetStaticMesh();
		FStaticMeshRenderData* renderData = mesh->RenderData.Get();

		// �׸� �����Ͱ� ���ٸ� Pass
		if (renderData->LODResources.Num() < 1) return FReply::Unhandled();

		FStaticMeshLODResources& lod = renderData->LODResources[0];
		FPositionVertexBuffer& vb = lod.VertexBuffers.PositionVertexBuffer;
		FStaticMeshVertexBuffer& meshVB = lod.VertexBuffers.StaticMeshVertexBuffer; // Normal, UV ���� ������
		FColorVertexBuffer& colorVB = lod.VertexBuffers.ColorVertexBuffer;
		FRawStaticIndexBuffer& ib = lod.IndexBuffer;

		// Position ���� ��ġ�� ���� ���� == ������ ���� ����
		//vb.GetNumVertices();
		uint32 vertexCount = vb.GetNumVertices();
		int32 indexCount = ib.GetNumIndices();

		//GLog->Logf(L"%d Mesh, Vertex Count : %d", i+1, vertexCount);
		//GLog->Logf(L"%d Mesh, Index Count : %d", i+1, indexCount);
		//GLog->Logf(L"%d Mesh, Triangle Count : %d", i+1, indexCount / 3);

		FStaticMesh_DetailData data;

		TArray<FColor> colors;
		colorVB.GetVertexColors(colors);

		if(colors.Num() < 1) // �־��� ������ ���ٸ� ������� ä���ֱ�
		{
			for (uint32 index = 0; index < vertexCount; index++)
				colors.Add(FColor(255, 255, 255, 255));
		}
		// ����(Extent) ã�� : min�� ���� ������ ��ȭ��, max�� ���� ������ �������� �����ϸ� �ȴ�.
		FVector minBounds = FVector(+MAX_FLT, +MAX_FLT, +MAX_FLT);
		FVector maxBounds = FVector(-MAX_FLT, -MAX_FLT, -MAX_FLT);

		for(uint32 index = 0; index < vertexCount; index++)
		{
			FVector position = vb.VertexPosition(index);

			if (position.X < minBounds.X) minBounds.X = position.X;
			if (position.Y < minBounds.Y) minBounds.Y = position.Y;
			if (position.Z < minBounds.Z) minBounds.Z = position.Z;

			if (position.X > maxBounds.X) maxBounds.X = position.X;
			if (position.Y > maxBounds.Y) maxBounds.Y = position.Y;
			if (position.Z > maxBounds.Z) maxBounds.Z = position.Z;

			data.Positions.Add(position);
			data.Normals.Add(meshVB.VertexTangentZ(i));
			data.Uvs.Add(meshVB.GetVertexUV(index, 0));
			data.Colors.Add(colors[index]);
		}

		float x = FMath::Abs(minBounds.X - maxBounds.X);
		float y = FMath::Abs(minBounds.Y - maxBounds.Y);
		float z = FMath::Abs(minBounds.Z - maxBounds.Z);

		data.Extent = FVector(x, y, z);
		data.Radius = FMath::Max(x, y);
		data.Radius = FMath::Max(data.Radius, z);

		//GLog->Logf(L"x : %0.1f, y : %0.1f, z : %0.1f", x, y, z);
		//GLog->Logf(L"Radius : %f", data.Radius);

		TArray<uint32> indices;
		ib.GetCopy(indices);
		data.Indices.Insert((int32*)indices.GetData(), indexCount, 0);

		///////////////////////////////////////////////////////////////////////

		FString path;
		FPaths::GetPath(path);

		IMainFrameModule& mainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
		// �ڵ�: �ش� ��ü�� �ĺ�������, �� ��ü�� �����ּ�
		// ex. ������ �ڵ� : �������� �ĺ���
		// mainFrame�� GetParentWindow�� Window Editor
		void* handle = mainFrame.GetParentWindow()->GetNativeWindow()->GetOSWindowHandle();
		// DWORD (Double WORD) �����ϱ�

		IDesktopPlatform* platform = FDesktopPlatformModule::Get();

		FString meshActorName = FString("SaveDialog - ") + meshActors[i]->GetName();

		TArray<FString> fileNames;
		platform->SaveFileDialog(handle, meshActorName, path, "", "Mesh Binary File(*.bin)|*.bin", EFileDialogFlags::None, fileNames);

		if (fileNames.Num() < 1) continue; // ������ ������ ���ٸ� Continue

		///////////////////////////////////////////////////////////////////////

		FBufferArchive buffer;
		buffer << data;

		FFileHelper::SaveArrayToFile(buffer, *fileNames[0]);
		buffer.FlushCache();
		buffer.Empty();

		///////////////////////////////////////////////////////////////////////

		FString text;
		for (int32 index = 0; index < data.Positions.Num(); index++)
		{
			text.Append(data.Positions[index].ToString() + ", ");
			text.Append(data.Normals[index].ToString() + ", ");
			text.Append(data.Uvs[index].ToString() + ", ");
			text.Append(data.Colors[index].ToString() + "\r\n");
		}

		FString textFileName = FPaths::GetBaseFilename(fileNames[0], false);
		FString textSaveName = textFileName + ".csv";

		//fileNames[0] : .. / .. / .. / .. / .. / .. / Users / USER / Desktop / Test.bin
		//textFileName : .. / .. / .. / .. / .. / .. / Users / USER / Desktop / Test
		//FPaths::GetCleanFilename(fileNames[0]) : Test.bin

		FFileHelper::SaveStringToFile(text, *textSaveName);

		FString str;
		str.Append(FPaths::GetBaseFilename(fileNames[0]));
		str.Append(" Saved.");
		//GLog->Log(str);
	}

	return FReply::Handled();
}

