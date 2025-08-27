#include "RTS/Systems/RTSGameMode.h"
#include "RTS/Systems/RTSPlayerController.h"
#include "RTS/Systems/RTSCameraPawn.h"
#include "Engine/Engine.h"
#include "NyangCraft.h"

ARTSGameMode::ARTSGameMode()
{
    PlayerControllerClass = ARTSPlayerController::StaticClass();
    DefaultPawnClass = ARTSCameraPawn::StaticClass();
}

void ARTSGameMode::AddMinerals(int32 Amount)
{
    const int32 Delta = FMath::Max(0, Amount);
    PlayerMinerals += Delta;
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.2f, FColor::Cyan,
            FString::Printf(TEXT("[RTS] 미네랄 +%d (총 %d)"), Delta, PlayerMinerals));
    }
    UE_LOG(LogNyangCraft, Log, TEXT("[RTS] Minerals +%d (total %d)"), Delta, PlayerMinerals);
}

