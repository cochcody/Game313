
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Resource_M.generated.h"

UCLASS()
class GAME313_API AResource_M : public AActor
{
    GENERATED_BODY()

public:
    AResource_M();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // === Resource Variables ===
    UPROPERTY(EditAnywhere)
    FString resourceName = "Wood";     // You will change this per Blueprint instance

    UPROPERTY(EditAnywhere)
    int resourceAmount = 50;           // Amount collected per tap

    UPROPERTY(EditAnywhere)
    int totalResource = 100;           // Total available in the node

    UPROPERTY()
    FText tempText;

    // === Components ===
    UPROPERTY(EditAnywhere)
    UTextRenderComponent* ResourceNameTxt;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* Mesh;
};
