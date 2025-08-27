# REQ020: Set Barracks Cost to 30

## Request Summary (EN)
Reduce Barracks mineral cost from 150 to 30 and ensure controller fallback matches.

## 원문 요약 (KO)
배럭 건설 비용을 150에서 30으로 낮추고, 컨트롤러의 기본값도 동일하게 수정.

## Changes Implemented
- `ARTSBuilding_Barracks`: `MineralCost = 30` in constructor.
- `ARTSPlayerController`: placement cost fallback changed to 30; still reads cost from CDO via `GetMineralCost()`.

## Files
- `Source/NyangCraft/RTS/Buildings/RTSBuilding_Barracks.cpp`
- `Source/NyangCraft/RTS/Systems/RTSPlayerController.cpp`

## Test Steps
1. Ensure you have ≥30 minerals.
2. Press B to enter build mode and left-click to place Barracks.
3. Minerals should deduct by 30; construction proceeds as before.

## Date
2025-08-27

