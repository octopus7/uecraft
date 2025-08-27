#include "RTS/Systems/RTSCameraPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

ARTSCameraPawn::ARTSCameraPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    RootComponent = CameraBoom;
    CameraBoom->TargetArmLength = 1500.f;
    CameraBoom->SetUsingAbsoluteRotation(true);
    CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
    CameraBoom->bDoCollisionTest = false;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;

    Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
    Movement->MaxSpeed = 2000.f;

    ZoomStep = 150.f;
    MinZoom = 600.f;
    MaxZoom = 3000.f;
}

void ARTSCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    // Polling via Tick using IsInputKeyDown to avoid EnhancedInput asset setup.
}

void ARTSCameraPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    FVector2D MoveDir(0.f, 0.f);
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (PC->IsInputKeyDown(EKeys::W)) MoveDir.Y += 1.f;
        if (PC->IsInputKeyDown(EKeys::S)) MoveDir.Y -= 1.f;
        if (PC->IsInputKeyDown(EKeys::D)) MoveDir.X += 1.f;
        if (PC->IsInputKeyDown(EKeys::A)) MoveDir.X -= 1.f;

        if (PC->WasInputKeyJustPressed(EKeys::MouseScrollUp))
        {
            CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength - ZoomStep, MinZoom, MaxZoom);
        }
        if (PC->WasInputKeyJustPressed(EKeys::MouseScrollDown))
        {
            CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength + ZoomStep, MinZoom, MaxZoom);
        }
    }

    if (!MoveDir.IsNearlyZero())
    {
        FVector WorldDir = FVector(MoveDir.Y, MoveDir.X, 0.f).GetClampedToMaxSize(1.f);
        AddMovementInput(WorldDir, 1.0f);
    }
}

