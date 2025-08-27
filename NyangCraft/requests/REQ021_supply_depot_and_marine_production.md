# REQ021: Supply Depot Construction and Marine Production

## Request Summary (EN)
Implement Supply Depot construction and Marine production. Add hotkeys for placement (D) and training (M), with simple training time at Barracks.

## 원문 요약 (KO)
보급창 건설과 마린 생산을 구현. D 키로 보급창 배치, M 키로 배럭에서 마린 훈련(간단한 훈련 시간 포함).

## Changes Implemented
- Supply Depot
  - `ARTSBuilding_SupplyDepot` with defaults: cost 20, build time 8s, max HP 600.
  - `RTSPlayerController`: D toggles placement mode; left-click places and spends minerals.
- Marine Production
  - `ARTSBuilding_Barracks`: training system with queue counter, training timer, and spawn on completion.
  - `RTSPlayerController`: M trains a Marine at the nearest completed Barracks to the cursor.
- Base Building
  - `ARTSBuilding_Base`: public getters for cost and construction state.

## Files
- `Source/NyangCraft/RTS/Buildings/RTSBuilding_Base.h/.cpp`
- `Source/NyangCraft/RTS/Buildings/RTSBuilding_SupplyDepot.h/.cpp`
- `Source/NyangCraft/RTS/Buildings/RTSBuilding_Barracks.h/.cpp`
- `Source/NyangCraft/RTS/Systems/RTSPlayerController.h/.cpp`

## Test Steps
1. Gather minerals.
2. Press D and left-click to place a Supply Depot; observe construction progress and completion.
3. Build a Barracks (B + click).
4. Press M (mouse over map) to train a Marine; after ~4s, the Marine spawns near the Barracks.

## Notes / Follow-ups
- Add per-unit mineral costs and supply usage/generation.
- Allow selecting buildings and showing production queues.
- Add placement validation and ghost previews.

## Date
2025-08-27

