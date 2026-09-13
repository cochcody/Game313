#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
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
    UFUNCTION()
    void MoveForward(float AxisValue);

    UFUNCTION()
    void MoveRight(float AxisValue);

    UFUNCTION()
    void StartJump();

    UFUNCTION()
    void StopJump();

    // Resource detection
    UFUNCTION()
    void FindObject();

    // Resource giving (declaration ONLY — no body here)
    UFUNCTION()
    void GiveResource(float amount, FString resourceType);

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

    // Resource counts
    UPROPERTY(EditAnywhere, Category = "Resources")
    int Wood;

    UPROPERTY(EditAnywhere, Category = "Resources")
    int Stone;

    UPROPERTY(EditAnywhere, Category = "Resources")
    int Berry;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
    TArray<int> ResourcesArray;

    UPROPERTY(EditAnywhere, Category = "Resources")
    TArray<FString> ResourcesNameArray;

    // Stat setters
    UFUNCTION(BlueprintCallable)
    void SetHealth(float amount);

    UFUNCTION(BlueprintCallable)
    void SetHunger(float amount);

    UFUNCTION(BlueprintCallable)
    void SetStamina(float amount);

    UPROPERTY(EditAnywhere, Category = "Effects")
    UMaterialInterface* hitDecal;


    UFUNCTION()
    void DecreaseStats();
};
