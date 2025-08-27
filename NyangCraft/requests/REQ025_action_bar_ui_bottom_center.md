# REQ025: Action Bar UI (Bottom-Center)

## Request Summary (EN)
Add a context-aware action bar at the bottom center with buttons for Build/Production and simple queue display.

## 원문 요약 (KO)
하단 중앙에 문맥형 버튼 UI(건설/생산/큐 표시)를 추가.

## Changes Implemented
- New `URTSActionBarWidget` (UUserWidget):
  - Shows Build buttons (Barracks/Depot) when no building selected or in build mode.
  - Shows Train buttons and Queue count when a Barracks/Command Center is selected.
  - Anchored bottom-center; polls controller to rebuild as context changes.
- `ARTSPlayerController`:
  - Exposes selection state and build modes to UI (getters and UI_* functions).
  - Adds `SelectedBuilding` and single-click selection for buildings.
  - Adds UI helpers: `UI_BuildBarracks/Depot`, `UI_TrainWorker/Marine`.

## Files
- `Source/NyangCraft/RTS/UI/RTSActionBarWidget.h/.cpp`
- `Source/NyangCraft/RTS/Systems/RTSPlayerController.h/.cpp`

## Test Steps
1. Run the game; observe action bar at bottom-center.
2. No selection: buttons "Build: Barracks" and "Build: Depot".
3. Click a Barracks: button "Train: Marine" and queue count.
4. Click a Command Center: button "Train: Worker" and queue count.
5. Click Build buttons to enter placement mode; left-click to place.

## Notes / Follow-ups
- Add ghost/validity preview for placement.
- Add progress bars and cancel buttons per queue item.
- Multi-selection aggregation rules if needed.

## Date
2025-08-27

