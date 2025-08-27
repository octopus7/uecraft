# REQ023: Worker Production at Command Center

## Request Summary (EN)
Enable producing Workers at the Command Center, with a training queue, supply checks, and a hotkey to trigger training.

## 원문 요약 (KO)
커맨드센터에서 워커 생산을 지원. 훈련 대기열/보급 체크 포함, 단축키로 훈련 가능.

## Changes Implemented
- `ARTSBuilding_CommandCenter`:
  - Added `QueueTrainWorker`, `TrainTimeWorker`, queue state, timer, and spawn on completion.
  - Consumes 1 supply per worker via `RTSGameMode::TryConsumeSupply`; retries if blocked.
- `ARTSPlayerController`:
  - W key trains a Worker at the nearest completed Command Center to the cursor.

## Files
- `Source/NyangCraft/RTS/Buildings/RTSBuilding_CommandCenter.h/.cpp`
- `Source/NyangCraft/RTS/Systems/RTSPlayerController.cpp`

## Test Steps
1. Ensure you have supply cap available.
2. Place or use an existing Command Center and wait for construction to complete.
3. Press W with the cursor over the map; after ~6s, a Worker spawns near the Command Center.
4. If supply is insufficient, observe the on-screen message and retry behavior.

## Notes / Follow-ups
- Can add mineral cost per worker if desired (currently supply-gated only to match Marine pattern).
- Add UI buttons when building selection UI exists.

## Date
2025-08-27

