# REQ019: Barracks Construction and Cost

## Request Summary (EN)
Add a simple Barracks construction flow: toggle build mode with the B key, click to place a Barracks at the cursor, spend minerals, and progress construction over time.

## 원문 요약 (KO)
B 키로 배럭 배치 모드 전환, 좌클릭으로 커서 위치에 배럭 건설. 미네랄 차감 및 시간이 지나며 건설 진행.

## Changes Implemented
- `ARTSBuilding_Base`
  - Root scene component, construction fields (MineralCost, BuildTime, BuildProgress, bUnderConstruction).
  - Ticks to advance progress, scales HP, and fires completion.
- `ARTSBuilding_Barracks`
  - Defaults: cost 150, build time 12s, max HP 1000.
- `ARTSGameMode`
  - `TrySpendMinerals(int32)` for affordability checks and deduction with HUD feedback.
- `ARTSPlayerController`
  - Poll key B to toggle Barracks placement mode; left-click to place at cursor.
  - Spawns Barracks and spends minerals via GameMode.
- HUD (existing) shows minerals so the deduction is visible.

## Files
- `Source/NyangCraft/RTS/Buildings/RTSBuilding_Base.h/.cpp`
- `Source/NyangCraft/RTS/Buildings/RTSBuilding_Barracks.h/.cpp`
- `Source/NyangCraft/RTS/Systems/RTSGameMode.h/.cpp`
- `Source/NyangCraft/RTS/Systems/RTSPlayerController.h/.cpp`

## Test Steps
1. Ensure you have ≥150 minerals (use gathering or temporarily call `AddMinerals`).
2. Press B → on-screen hint shows build mode.
3. Left-click on the map ground → Barracks spawns, minerals deduct, construction starts.
4. After ~12s, completion message appears; HP reaches MaxHP.

## Notes / Follow-ups
- Add placement validity checks (navmesh, collision, build radius) and a translucent ghost preview.
- Bind additional building hotkeys and a cancel key (Esc/right-click).
- Show build progress bar over the building.

## Date
2025-08-27

