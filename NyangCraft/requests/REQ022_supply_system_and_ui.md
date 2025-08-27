# REQ022: Supply System and HUD UI

## Request Summary (EN)
Connect a simple supply system to unit production and display supply on the HUD.

## 원문 요약 (KO)
보급(인구) 시스템을 생산과 연결하고, HUD에 보급 정보를 표시.

## Changes Implemented
- `ARTSGameMode`: supply tracking and API
  - Fields: `PlayerSupplyUsed`, `PlayerSupplyCap` (base 10)
  - `AddSupplyCap`, `TryConsumeSupply`, `ReleaseSupply`, getters
- `ARTSBuilding_SupplyDepot`: on construction complete, adds +8 supply cap.
- `ARTSBuilding_Barracks`: Marine training now checks supply and waits if blocked; uses 1 supply per Marine.
- `URTSHUDWidget`: HUD shows `Minerals: N    Supply: U/C` at top-right.

## Files
- `Source/NyangCraft/RTS/Systems/RTSGameMode.h/.cpp`
- `Source/NyangCraft/RTS/Buildings/RTSBuilding_SupplyDepot.h/.cpp`
- `Source/NyangCraft/RTS/Buildings/RTSBuilding_Barracks.h/.cpp`
- `Source/NyangCraft/RTS/UI/RTSHUDWidget.h/.cpp`

## Test Steps
1. Start with base cap 10; HUD shows `Supply: 0/10`.
2. Build a Supply Depot; on completion cap increases to 18.
3. Train Marines; each consumes 1 supply; HUD used value increases.
4. If supply is insufficient, barracks shows a red message and retries.

## Notes / Follow-ups
- Release supply on unit death/despawn not yet implemented.
- Unit mineral cost and supply cost can be exposed on per-unit basis.
- We can add building selection UI to show queue/progress.

## Date
2025-08-27

