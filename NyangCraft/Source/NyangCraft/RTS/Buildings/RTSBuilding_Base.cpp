#include "RTS/Buildings/RTSBuilding_Base.h"
#include "Components/SceneComponent.h"
#include "Engine/Engine.h"
#include "NyangCraft.h"

ARTSBuilding_Base::ARTSBuilding_Base()
{
    PrimaryActorTick.bCanEverTick = true;
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);
    HP = 1; // start fragile until built
    bUnderConstruction = true;
}

void ARTSBuilding_Base::BeginPlay()
{
    Super::BeginPlay();
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Silver,
            FString::Printf(TEXT("[RTS] 건설 시작: %s (%.0fs)"), *GetName(), BuildTime));
    }
}

void ARTSBuilding_Base::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (!bUnderConstruction) return;
    if (BuildTime <= 0.f)
    {
        BuildProgress = 1.f;
    }
    else
    {
        BuildProgress = FMath::Min(1.f, BuildProgress + (DeltaSeconds / BuildTime));
    }
    // Scale HP with progress
    HP = FMath::Max(1, FMath::RoundToInt(MaxHP * BuildProgress));
    if (BuildProgress >= 1.f)
    {
        bUnderConstruction = false;
        HP = MaxHP;
        OnConstructionCompleted();
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green,
                FString::Printf(TEXT("[RTS] 건설 완료: %s"), *GetName()));
        }
        UE_LOG(LogNyangCraft, Log, TEXT("[RTS] Construction completed: %s"), *GetName());
    }
}

void ARTSBuilding_Base::OnConstructionCompleted()
{
    // derived classes may override
}

