#include "Weapons/CWeapon.h"
#include "Global.h"
#include "CBullet.h"
#include "Characters/CPlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Particles/ParticleSystem.h"
#include "Widgets/CUserWidget_CrossHair.h"
#include "Animation/AnimMontage.h"

void FWeaponAimData::SetData(ACharacter* InOwner)
{
	USpringArmComponent* springArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	springArm->TargetArmLength = TargetArmLength;
	springArm->SocketOffset = SocketOffset;
}

void FWeaponAimData::SetDataByNoneCurve(ACharacter* InOwner)
{
	USpringArmComponent* springArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	springArm->TargetArmLength = TargetArmLength;
	springArm->SocketOffset = SocketOffset;

	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(InOwner);
	camera->FieldOfView = FieldOfView;
}

///////////////////////////////////////////////////////////////////////////////

ACWeapon::ACWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Root);

	CHelpers::CreateActorComponent<UTimelineComponent>(this, &Timeline, "Timeline");

	CHelpers::GetAsset<UMaterialInstanceConstant>(&HitDecal, "MaterialInstanceConstant'/Game/Materials/M_Decal_Inst.M_Decal_Inst'");
	CHelpers::GetAsset<UParticleSystem>(&HitParticle, "ParticleSystem'/Game/Effects/P_Impact_Default.P_Impact_Default'");

	CHelpers::GetAsset<UCurveFloat>(&AimCurve, "CurveFloat'/Game/Weapons/Curve_Aim.Curve_Aim'");

	CHelpers::GetAsset<UParticleSystem>(&FlashParticle, "ParticleSystem'/Game/Effects/P_Muzzleflash.P_Muzzleflash'");
	CHelpers::GetAsset<UParticleSystem>(&EjectParticle, "ParticleSystem'/Game/Effects/P_Eject_bullet.P_Eject_bullet'");
	CHelpers::GetAsset<USoundWave>(&FireSound, "SoundWave'/Game/Sounds/S_RifleShoot.S_RifleShoot'");

	CHelpers::GetClass<ACBullet>(&BulletClass, "Blueprint'/Game/Weapons/BP_CBullet.BP_CBullet_C'");
}

void ACWeapon::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACPlayer>(GetOwner());

	if (HolsterSocketName.IsValid())
		AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HolsterSocketName);

	BaseData.SetDataByNoneCurve(Owner);

	if(!!AimCurve)
	{
		FOnTimelineFloat timeline;
		timeline.BindUFunction(this, "OnAiming");

		Timeline->AddInterpFloat(AimCurve, timeline);
		Timeline->SetLooping(false);
		Timeline->SetPlayRate(AimingSpeed);
	}

	if(!!CrossHairClass)
	{
		CrossHair = CreateWidget<UCUserWidget_CrossHair, APlayerController>(Owner->GetController<APlayerController>(), CrossHairClass);
		CrossHair->AddToViewport();
		CrossHair->SetVisibility(ESlateVisibility::Hidden);
		CrossHair->UpdateSpreadRange(CurrSpreadRadius, MaxSpreadAlignment);
	}

	// 총알 개수 초기화
	CurrMagazineCount = MaxMagazineCount;


}

void ACWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 한발이라도 쏜 적이 있다면
	if(LastAddSpreadTime >= 0.0f)
	{
		if(GetWorld()->GetTimeSeconds() - LastAddSpreadTime >= AutoFireInterval + 0.25f)
		{
			CurrSpreadRadius = 0.0f;
			LastAddSpreadTime = 0.0f;

			if (!!CrossHair)
				CrossHair->UpdateSpreadRange(CurrSpreadRadius, MaxSpreadAlignment);
		}
	}
}

bool ACWeapon::CanEquip()
{
	bool b = false;
	// 하나라도 true라면 Equip할 수 없음
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;

	return !b;
}

void ACWeapon::Equip()
{
	bEquipping = true;

	if (!!EquipMontage)
		Owner->PlayAnimMontage(EquipMontage, EquipMontage_PlayRate);
}

void ACWeapon::Begin_Equip()
{
	if (RightHandSocketName.IsValid())
		AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), RightHandSocketName);
}

void ACWeapon::End_Equip()
{
	bEquipping = false;

	if(!!CrossHair)
		CrossHair->SetVisibility(ESlateVisibility::Visible);
}

bool ACWeapon::CanUnequip()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;

	return !b;
}

void ACWeapon::Unequip()
{
	if(HolsterSocketName.IsValid())
		AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HolsterSocketName);

	if (!!CrossHair)
		CrossHair->SetVisibility(ESlateVisibility::Hidden);
}

bool ACWeapon::CanAim()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bInAim;

	return !b;
}

void ACWeapon::Begin_Aim()
{
	bInAim = true;

	if(!!AimCurve)
	{
		Timeline->PlayFromStart();
		AimData.SetData(Owner);

		return;
	}

	AimData.SetDataByNoneCurve(Owner);
}

void ACWeapon::End_Aim()
{
	CheckFalse(bInAim);

	bInAim = false;

	if (!!AimCurve)
	{
		Timeline->ReverseFromEnd();
		BaseData.SetData(Owner);

		return;
	}

	BaseData.SetDataByNoneCurve(Owner);
}

bool ACWeapon::CanFire()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	b |= bFiring;

	return !b;
}

void ACWeapon::Begin_Fire()
{
	bFiring = true;

	if(bAutoFire)
	{
		GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this,
			&ACWeapon::OnFiring, AutoFireInterval, true, 0);
		return;
	}

	OnFiring();
}

void ACWeapon::End_Fire()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(AutoFireHandle))
		GetWorld()->GetTimerManager().ClearTimer(AutoFireHandle);

	bFiring = false;
}

void ACWeapon::OnFiring()
{
	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(Owner);
	FVector direction = camera->GetForwardVector();
	FTransform transform = camera->GetComponentToWorld();

	FVector start = transform.GetLocation() + direction;

	// 랜덤 범위로 탄착군 형성
	direction = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(direction, RecoilAngle);
	FVector end = transform.GetLocation() + direction * HitDistance;

	//DrawDebugLine(GetWorld(), start, end, FColor::Red, true, 5);

	TArray<AActor*> ignores;
	ignores.Add(Owner);

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end,
		ETraceTypeQuery::TraceTypeQuery1, false, ignores,
		EDrawDebugTrace::None, hitResult, true);

	// 충돌된 액터가 존재
	if (hitResult.bBlockingHit)
	{
		// 데칼 그리기 : 벽에 총알 박힌 자국
		if (!!HitDecal)
		{
			FRotator rotator = hitResult.ImpactNormal.Rotation();

			UDecalComponent* decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), HitDecal,
				FVector(5), hitResult.Location, rotator, 10.0f);
			decal->SetFadeScreenSize(0); // LOD : 멀리가면 안보이는 기능을 최대한 끈것
		}

		// 파티클 출력하기 : 벽에 총알 충돌 시 작은 폭발 + 연기
		if (!!HitParticle)
		{
			FRotator impactorRotation = UKismetMathLibrary::FindLookAtRotation(hitResult.Location, hitResult.TraceStart);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, hitResult.Location, impactorRotation);
		}
		CLog::Log(hitResult.GetActor()->GetName());
	}

	// 파티클 : 총구에서 화염이 뿜어져 나오는 것
	if (!!FlashParticle)
		UGameplayStatics::SpawnEmitterAttached(FlashParticle, Mesh, "Muzzle",
			FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);

	// 파티클 : 발사된 탄피가 튕겨져 나오는 것
	if (!!EjectParticle)
		UGameplayStatics::SpawnEmitterAttached(EjectParticle, Mesh, "Eject",
			FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);

	// 총구 위치
	FVector muzzleLocation = Mesh->GetSocketLocation("Muzzle");

	// 총소리
	if (!!FireSound)
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, muzzleLocation);

	// 카메라 Shake - 총의 반동
	if(!!CameraShakeClass)
	{
		APlayerController* controller = Owner->GetController<APlayerController>();

		if (!!controller)
			controller->PlayerCameraManager->StartCameraShake(CameraShakeClass);
	}
	// 총의 반동에 의해 총구가 조금씩 위로 올라감
	Owner->AddControllerPitchInput(-RecoilRate * UKismetMathLibrary::RandomFloatInRange(0.8, 1.2));

	// CrossHair 퍼뜨리기
	if(CurrSpreadRadius <= 1.0f)
	{
		CurrSpreadRadius += SpreadSpeed * GetWorld()->GetDeltaSeconds();

		if(!!CrossHair)
			CrossHair->UpdateSpreadRange(CurrSpreadRadius, MaxSpreadAlignment);
	}
	// 게임 시작으로부터 현재 시간
	LastAddSpreadTime = GetWorld()->GetTimeSeconds();

	// 총알 생성
	if(!!BulletClass)
	{
		FVector location = Mesh->GetSocketLocation("Muzzle_Bullet");

		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ACBullet* bullet = GetWorld()->SpawnActor<ACBullet>(BulletClass, location, direction.Rotation(), params);

		if (!!bullet)
			bullet->Shoot(direction);
	}

	if(CurrMagazineCount >= 1)
	{
		CurrMagazineCount--;
	}
	else
	{
		if (CanReload())
			Reload();
	}
}

void ACWeapon::ToggleAutoFire()
{
	bAutoFire = !bAutoFire;
}

bool ACWeapon::CanReload()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;

	return !b;
}

void ACWeapon::Reload()
{
	bReload = true;

	End_Aim();
	End_Fire();

	if (!!ReloadMontage)
		Owner->PlayAnimMontage(ReloadMontage, ReloadMontage_PlayRate);

}

void ACWeapon::Eject_Magazine()
{
}

void ACWeapon::Spawn_Magazine()
{
}

void ACWeapon::Load_Magazine()
{
}

void ACWeapon::End_Reload()
{
	bReload = false;
}


void ACWeapon::OnAiming(float Output)
{
	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(Owner);
	camera->FieldOfView = FMath::Lerp(AimData.FieldOfView, BaseData.FieldOfView, Output);
}

