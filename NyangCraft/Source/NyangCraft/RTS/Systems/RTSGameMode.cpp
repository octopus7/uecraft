#include "RTS/Systems/RTSGameMode.h"
#include "RTS/Systems/RTSPlayerController.h"
#include "RTS/Systems/RTSCameraPawn.h"

ARTSGameMode::ARTSGameMode()
{
    PlayerControllerClass = ARTSPlayerController::StaticClass();
    DefaultPawnClass = ARTSCameraPawn::StaticClass();
}

