#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "BuildingPart.h"
#include "Resource_M.h"
#include "PlayerChar.generated.h"

UCLASS()
class GAME313_API APlayerChar : public ACharacter
{
    GENERATED_BODY()

public:
    APlayerChar();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Movement
    void MoveForward(float AxisValue);
    void MoveRight(float AxisValue);
    void StartJump();
    void StopJump();

    // Camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UCameraComponent* PlayerCamComp;

    // Player Stats
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
    float Health = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
    float Hunger = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
    float Stamina = 100.0f;

    UFUNCTION(BlueprintCallable)
    void SetHealth(float amount);

    UFUNCTION(BlueprintCallable)
    void SetHunger(float amount);

    UFUNCTION(BlueprintCallable)
    void SetStamina(float amount);

    UFUNCTION()
    void DecreaseStats();

    // Resource arrays
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
    TArray<int> ResourceArray;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    TArray<int> BuildingArray;

    // Effects
    UPROPERTY(EditAnywhere, Category = "Effects")
    UParticleSystem* HitEffect;

    UPROPERTY(EditAnywhere, Category = "Effects")
    UMaterialInterface* hitDecal;

    // Resource detection
    UFUNCTION()
    void FindObject();

    // Give resource
    UFUNCTION()
    void GiveResource(float amount, FString resourceType);

    // Update resources after building
    UFUNCTION(BlueprintCallable)
    void UpdateResources(float woodAmount, float stoneAmount, FString buildingObject);

    // Spawn building
    UFUNCTION(BlueprintCallable)
    void SpawnBuilding(int BuildingID, bool& isSuccess);

    // Rotate building preview
    UFUNCTION()
    void RotateBuilding();

    // Building mode flag
    UPROPERTY()
    bool bIsBuilding;

    // Preview building part
    UPROPERTY()
    ABuildingPart* SpawnedPart;

    // Building part class
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class ABuildingPart> BuildingPartClass;
};
