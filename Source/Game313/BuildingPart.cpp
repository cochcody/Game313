// Fill out your copyright notice in the Description page of Project Settings.
#include "BuildingPart.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

ABuildingPart::ABuildingPart()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    PivotArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Pivot Arrow"));

    RootComponent = PivotArrow;
    Mesh->SetupAttachment(PivotArrow);
}

void ABuildingPart::BeginPlay()
{
    Super::BeginPlay();
}

void ABuildingPart::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

