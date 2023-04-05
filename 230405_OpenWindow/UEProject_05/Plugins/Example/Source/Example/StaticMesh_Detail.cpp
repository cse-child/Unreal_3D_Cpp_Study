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

	mesh.AddCustomRow(FText::FromString("Color")) // 검색 기준
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Color"))
	]
	.ValueContent()
	.VAlign(VAlign_Center)		// 가운데 정렬
	.MaxDesiredWidth(250)		// 최대 넓이
	[
		SNew(SButton)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)	// 꽉 채운 상태로 시작
		.OnClicked(this, &FStaticMesh_Detail::OnClicked_Paint)
		.Content()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Paint"))
		]
	];

	mesh.AddCustomRow(FText::FromString("SaveMesh")) // 검색 기준
		.NameContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Mesh"))
		]
	.ValueContent()
		.VAlign(VAlign_Center)		// 가운데 정렬
		.MaxDesiredWidth(250)		// 최대 넓이
		[
			SNew(SButton)
			.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)	// 꽉 채운 상태로 시작
		.OnClicked(this, &FStaticMesh_Detail::OnClicked_Save)
		.Content()
		[
			SNew(STextBlock)
			.Text(FText::FromString("Save"))
		]
		];

	// 클릭한 Object의 List를 반환
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

	// 선택한 Mesh 전부 meshActors에 저장하기
	TArray<ACStaticMesh*> meshActors;
	for(TWeakObjectPtr<UObject> obj : Objects)
	{
		ACStaticMesh* meshActor = Cast<ACStaticMesh>(obj);
		if (meshActor == nullptr) continue;

		meshActors.Add(meshActor);
	}

	/* Message Dialog 테스트 */
	//{
	//	FText title = FText::FromString("Warning");
	//	FText context = FText::FromString(FString::Printf(L"Selected Num : %d", meshActors.Num()));
	//	FMessageDialog::Debugf(context, &title);
	//}

	// 선택된 Mesh들의 Save 기능 구현
	for(int32 i = 0; i < meshActors.Num(); i++)
	{
		UActorComponent* actorComponent = meshActors[i]->GetComponentByClass(UStaticMeshComponent::StaticClass());
		UStaticMeshComponent* meshComponent = Cast<UStaticMeshComponent>(actorComponent);

		// mesh Component가 존재하지 않아서 처리할 버튼 명령이 없다면 다음 레이어로 Pass
		if (meshComponent == nullptr) FReply::Unhandled();

		// 실제로 화면에 그려지는 것은 Mesh 컴포넌트 안에있는 Static Mesh
		UStaticMesh* mesh = meshComponent->GetStaticMesh();
		FStaticMeshRenderData* renderData = mesh->RenderData.Get();

		// 그릴 데이터가 없다면 Pass
		if (renderData->LODResources.Num() < 1) return FReply::Unhandled();

		FStaticMeshLODResources& lod = renderData->LODResources[0];
		FPositionVertexBuffer& vb = lod.VertexBuffers.PositionVertexBuffer;
		FStaticMeshVertexBuffer& meshVB = lod.VertexBuffers.StaticMeshVertexBuffer; // Normal, UV 같이 들어가있음
		FColorVertexBuffer& colorVB = lod.VertexBuffers.ColorVertexBuffer;
		FRawStaticIndexBuffer& ib = lod.IndexBuffer;

		// Position 버퍼 위치의 정점 개수 == 버퍼의 원소 개수
		//vb.GetNumVertices();
		uint32 vertexCount = vb.GetNumVertices();
		int32 indexCount = ib.GetNumIndices();

		//GLog->Logf(L"%d Mesh, Vertex Count : %d", i+1, vertexCount);
		//GLog->Logf(L"%d Mesh, Index Count : %d", i+1, indexCount);
		//GLog->Logf(L"%d Mesh, Triangle Count : %d", i+1, indexCount / 3);

		FStaticMesh_DetailData data;

		TArray<FColor> colors;
		colorVB.GetVertexColors(colors);

		if(colors.Num() < 1) // 주어진 색상이 없다면 흰색으로 채워넣기
		{
			for (uint32 index = 0; index < vertexCount; index++)
				colors.Add(FColor(255, 255, 255, 255));
		}
		// 부피(Extent) 찾기 : min을 가장 앞쪽의 좌화단, max를 가장 뒤쪽의 우상단으로 설정하며 된다.
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
		// 핸들: 해당 객체의 식별자이자, 그 객체의 시작주소
		// ex. 윈도우 핸들 : 윈도우의 식별자
		// mainFrame의 GetParentWindow는 Window Editor
		void* handle = mainFrame.GetParentWindow()->GetNativeWindow()->GetOSWindowHandle();
		// DWORD (Double WORD) 공부하기

		IDesktopPlatform* platform = FDesktopPlatformModule::Get();

		FString meshActorName = FString("SaveDialog - ") + meshActors[i]->GetName();

		TArray<FString> fileNames;
		platform->SaveFileDialog(handle, meshActorName, path, "", "Mesh Binary File(*.bin)|*.bin", EFileDialogFlags::None, fileNames);

		if (fileNames.Num() < 1) continue; // 선택한 파일이 없다면 Continue

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

