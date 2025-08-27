# REQ017: Mineral HUD Widget

## Request Summary (EN)
Add a minimal HUD widget that displays the player’s current Mineral count on screen during RTS gameplay.

## 원문 요약 (KO)
RTS 플레이 중 화면에 미네랄 보유량을 표시하는 간단한 HUD 위젯 추가.

## Changes Implemented
- New `URTSHUDWidget` (UUserWidget) that draws a translucent panel and text "Minerals: N" at the top-left.
- Widget updates each tick by reading `ARTSGameMode::GetMinerals()`.
- Added to viewport from `ARTSPlayerController::BeginPlay()` alongside selection widget.

## Files
- `Source/NyangCraft/RTS/UI/RTSHUDWidget.h`
- `Source/NyangCraft/RTS/UI/RTSHUDWidget.cpp`
- `Source/NyangCraft/RTS/Systems/RTSPlayerController.h`
- `Source/NyangCraft/RTS/Systems/RTSPlayerController.cpp`

## Test Steps
1. Build and run RTS map.
2. Select a worker, order gathering on a mineral.
3. Observe top-left HUD: "Minerals: X" updates as deposits complete.

## Notes / Follow-ups
- Styling is tunable in widget properties (panel color, text color, font size, padding/offset).
- We can replace the text with an icon + number, add animations, or switch to event-driven updates if preferred.

## Date
2025-08-27

