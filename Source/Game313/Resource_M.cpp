// Fill out your copyright notice in the Description page of Project Settings.
#include "Resource_M.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"

AResource_M::AResource_M()
{
    PrimaryActorTick.bCanEverTick = true;

    ResourceNameTxt = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Render"));
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

    RootComponent = Mesh;
    ResourceNameTxt->SetupAttachment(Mesh);
}

void AResource_M::BeginPlay()
{
    Super::BeginPlay();

    tempText = FText::FromString(resourceName);
    ResourceNameTxt->SetText(tempText);
}

void AResource_M::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

