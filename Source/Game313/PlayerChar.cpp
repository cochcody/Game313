#include "PlayerChar.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Resource_M.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

APlayerChar::APlayerChar()
{
    PrimaryActorTick.bCanEverTick = true;

    PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
    PlayerCamComp->SetupAttachment(GetMesh());
    PlayerCamComp->bUsePawnControlRotation = true;

    bUseControllerRotationYaw = true;

    ResourceArray.SetNum(3);   // Wood, Stone
    BuildingArray.SetNum(3);   // Wall, House/Floor, Ceiling

    bIsBuilding = false;
    SpawnedPart = nullptr;
}

void APlayerChar::BeginPlay()
{
    Super::BeginPlay();

    FTimerHandle StatsTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &APlayerChar::DecreaseStats, 2.0f, true);
}

void APlayerChar::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsBuilding && SpawnedPart)
    {
        FVector StartLocation = PlayerCamComp->GetComponentLocation();
        FVector Direction = PlayerCamComp->GetForwardVector() * 100.0f;
        FVector EndLocation = StartLocation + Direction;

        SpawnedPart->SetActorLocation(EndLocation);
    }
}

void APlayerChar::MoveForward(float AxisValue)
{
    if (Controller && AxisValue != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, AxisValue);
    }
}

void APlayerChar::MoveRight(float AxisValue)
{
    if (Controller && AxisValue != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, AxisValue);
    }
}

void APlayerChar::StartJump()
{
    Jump();
}

void APlayerChar::StopJump()
{
    StopJumping();
}

void APlayerChar::FindObject()
{
    if (bIsBuilding)
        return;

    FHitResult HitResult;

    FVector StartLocation = PlayerCamComp->GetComponentLocation();
    FVector Direction = PlayerCamComp->GetForwardVector() * 500.0f;
    FVector EndLocation = StartLocation + Direction;

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.bTraceComplex = true;

    if (GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        ECC_Visibility,
        QueryParams))
    {
        AResource_M* HitResource = Cast<AResource_M>(HitResult.GetActor());

        if (HitResource)
        {
            FString HitName = HitResource->resourceName;
            int resourceValue = HitResource->resourceAmount;

            HitResource->totalResource -= resourceValue;

            if (HitResource->totalResource > 0)
            {
                GiveResource(resourceValue, HitName);

                if (GEngine)
                    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Resource Collected"));

                UGameplayStatics::SpawnEmitterAtLocation(
                    GetWorld(),
                    HitEffect,
                    HitResult.Location,
                    FRotator::ZeroRotator
                );
            }
            else
            {
                HitResource->Destroy();

                if (GEngine)
                    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Depleted"));
            }
        }
    }
    else
    {
        bIsBuilding = false;
    }
}

void APlayerChar::GiveResource(float amount, FString resourceType)
{
    if (resourceType == "Wood")
        ResourceArray[0] += amount;

    if (resourceType == "Stone")
        ResourceArray[1] += amount;

    if (resourceType == "Berry")
        ResourceArray[2] += amount;
}

void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);

    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

    PlayerInputComponent->BindAction("JumpEvent", IE_Pressed, this, &APlayerChar::StartJump);
    PlayerInputComponent->BindAction("JumpEvent", IE_Released, this, &APlayerChar::StopJump);

    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerChar::FindObject);
    PlayerInputComponent->BindAction("PutPart", IE_Pressed, this, &APlayerChar::RotateBuilding);
}

void APlayerChar::SetHealth(float amount)
{
    if (Health + amount < 100)
        Health += amount;
}

void APlayerChar::SetHunger(float amount)
{
    if (Hunger + amount < 100)
        Hunger += amount;
}

void APlayerChar::SetStamina(float amount)
{
    if (Stamina + amount < 100)
        Stamina += amount;
}

void APlayerChar::DecreaseStats()
{
    if (Hunger > 0)
        SetHunger(-1.0f);

    SetStamina(10.0f);

    if (Hunger <= 0)
        SetHealth(-3.0f);
}

void APlayerChar::UpdateResources(float woodAmount, float stoneAmount, FString buildingObject)
{
    if (stoneAmount != ResourceArray[1])
    {
        ResourceArray[1] -= stoneAmount;
    }

    if (buildingObject == "wall")
        BuildingArray[0] += 1;

    if (buildingObject == "Floor")
        BuildingArray[1] += 1;

    if (buildingObject == "Ceiling")
        BuildingArray[2] += 1;
}

void APlayerChar::SpawnBuilding(int BuildingID, bool& isSuccess)
{
    if (!bIsBuilding)
    {
        if (BuildingArray[BuildingID] >= 1)
        {
            bIsBuilding = true;

            FActorSpawnParameters SpawnParams;

            FVector StartLocation = PlayerCamComp->GetComponentLocation();
            FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
            FVector EndLocation = StartLocation + Direction;

            FRotator myRot(0, 0, 0);

            BuildingArray[BuildingID] -= 1;

            SpawnedPart = GetWorld()->SpawnActor<ABuildingPart>(
                BuildingPartClass,
                EndLocation,
                myRot,
                SpawnParams
            );

            isSuccess = true;
            return;
        }
    }

    isSuccess = false;
}

void APlayerChar::RotateBuilding()
{
    if (bIsBuilding && SpawnedPart)
    {
        SpawnedPart->AddActorWorldRotation(FRotator(0, 90, 0));
    }
}
