// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHoleActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "FPSCharacter.h"

// Sets default values
AFPSBlackHoleActor::AFPSBlackHoleActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(MeshComp);
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFPSBlackHoleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSBlackHoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> hits;
	SphereComp->GetOverlappingComponents(hits);
	for (auto &comp : hits) {
		AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(comp->GetOwner());
		if (MyCharacter) {
			continue;
		}
		if (!comp->IsSimulatingPhysics()) {
			continue;
		}
		FVector v = GetActorLocation() - comp->GetOwner()->GetActorLocation();
		v.Normalize();
		comp->AddRadialForce(GetActorLocation(), SphereComp->GetScaledSphereRadius(), -2000.0, ERadialImpulseFalloff::RIF_Constant, true);
	}

}

