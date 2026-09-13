#include "PlayerChar.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Resource_M.h"
#include "Kismet/GameplayStatics.h"

APlayerChar::APlayerChar()
{
    PrimaryActorTick.bCanEverTick = true;

    // Camera setup
    PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
    PlayerCamComp->SetupAttachment(GetMesh());
    PlayerCamComp->bUsePawnControlRotation = true;

    bUseControllerRotationYaw = true;

    // Resource arrays
    ResourcesArray.SetNum(3);

    ResourcesNameArray.Add(TEXT("Wood"));
    ResourcesNameArray.Add(TEXT("Stone"));
    ResourcesNameArray.Add(TEXT("Berry"));
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
    FHitResult HitResult;

    FVector StartLocation = PlayerCamComp->GetComponentLocation();
    FVector Direction = PlayerCamComp->GetForwardVector() * 1500.0f;
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
            // ⭐ Stamina requirement
            if (Stamina > 5.0f)
            {
                FString HitName = HitResource->resourceName;
                int resourceValue = HitResource->resourceAmount;

                HitResource->totalResource -= resourceValue;

                if (HitResource->totalResource > resourceValue)
                {
                    GiveResource(resourceValue, HitName);

                    if (GEngine)
                        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Collected"));

                    // ⭐ Spawn decal at hit location
                    UGameplayStatics::SpawnDecalAtLocation(
                        GetWorld(),
                        hitDecal,
                        FVector(10.0f, 10.0f, 10.0f),
                        HitResult.Location,
                        FRotator(-90, 0, 0),
                        2.0f
                    );

                    // ⭐ Stamina drain (your SetStamina adds)
                    SetStamina(-5.0f);
                }
                else
                {
                    HitResource->Destroy();

                    if (GEngine)
                        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Depleted"));
                }
            }
        }
    }
}

void APlayerChar::GiveResource(float amount, FString resourceType)
{
    if (resourceType == "Wood")
        ResourcesArray[0] += amount;

    if (resourceType == "Stone")
        ResourcesArray[1] += amount;

    if (resourceType == "Berry")
        ResourcesArray[2] += amount;
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
