#include "RTS/Resources/RTSResource_Mineral.h"
#include "Engine/Engine.h"
#include "NyangCraft.h"

ARTSResource_Mineral::ARTSResource_Mineral()
{
    Amount = 1500; // default mineral amount
}

int32 ARTSResource_Mineral::Harvest(int32 Request)
{
    const int32 Taken = FMath::Clamp(Request, 0, Amount);
    Amount -= Taken;
    if (GEngine && Taken > 0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 0.8f, FColor::Blue,
            FString::Printf(TEXT("[RTS] 미네랄 채굴 %d (잔여 %d)"), Taken, Amount));
    }
    UE_LOG(LogNyangCraft, Verbose, TEXT("[RTS] Mineral harvested %d (remain %d)"), Taken, Amount);
    return Taken;
}

bool ARTSResource_Mineral::IsDepleted() const
{
    return Amount <= 0;
}

