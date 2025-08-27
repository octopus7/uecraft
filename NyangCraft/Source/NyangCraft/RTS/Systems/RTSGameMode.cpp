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

void ARTSGameMode::AddSupplyCap(int32 Amount)
{
    const int32 Delta = FMath::Max(0, Amount);
    PlayerSupplyCap += Delta;
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.2f, FColor::Cyan,
            FString::Printf(TEXT("[RTS] 보급 한도 +%d (총 %d)"), Delta, PlayerSupplyCap));
    }
    UE_LOG(LogNyangCraft, Log, TEXT("[RTS] Supply cap +%d (total %d)"), Delta, PlayerSupplyCap);
}

bool ARTSGameMode::TryConsumeSupply(int32 Amount)
{
    const int32 Need = FMath::Max(0, Amount);
    if (PlayerSupplyUsed + Need > PlayerSupplyCap)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.2f, FColor::Red,
                FString::Printf(TEXT("[RTS] 보급 부족: %d/%d"), PlayerSupplyUsed, PlayerSupplyCap));
        }
        return false;
    }
    PlayerSupplyUsed += Need;
    return true;
}

void ARTSGameMode::ReleaseSupply(int32 Amount)
{
    const int32 Give = FMath::Max(0, Amount);
    PlayerSupplyUsed = FMath::Max(0, PlayerSupplyUsed - Give);
}

bool ARTSGameMode::TrySpendMinerals(int32 Cost)
{
    const int32 Need = FMath::Max(0, Cost);
    if (PlayerMinerals < Need)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.2f, FColor::Red,
                FString::Printf(TEXT("[RTS] 미네랄 부족: 필요 %d / 보유 %d"), Need, PlayerMinerals));
        }
        UE_LOG(LogNyangCraft, Warning, TEXT("[RTS] Not enough minerals: need %d / have %d"), Need, PlayerMinerals);
        return false;
    }
    PlayerMinerals -= Need;
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan,
            FString::Printf(TEXT("[RTS] 미네랄 -%d (잔여 %d)"), Need, PlayerMinerals));
    }
    return true;
}

