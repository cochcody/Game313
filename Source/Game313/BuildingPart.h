// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "BuildingPart.generated.h"

UCLASS()
class GAME313_API ABuildingPart : public AActor
{
    GENERATED_BODY()

public:
    ABuildingPart();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere)
    UArrowComponent* PivotArrow;
};
