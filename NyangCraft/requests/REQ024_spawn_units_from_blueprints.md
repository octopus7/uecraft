# REQ024: Spawn Units from Blueprints

## Request Summary (EN)
Allow Barracks and Command Center to spawn blueprint versions of Marines and Workers via configurable class properties.

## 원문 요약 (KO)
배럭/커맨드센터가 C++ 기본 클래스 대신, 설정한 블루프린트 클래스로 마린/워커를 생성하도록 지원.

## Changes Implemented
- Barracks
  - `UPROPERTY EditDefaultsOnly TSubclassOf<ARTSMarine> MarineClass` added.
  - Spawning uses `MarineClass` if set; otherwise falls back to `ARTSMarine::StaticClass()`.
- Command Center
  - `UPROPERTY EditDefaultsOnly TSubclassOf<ARTSWorker> WorkerClass` added.
  - Spawning uses `WorkerClass` if set; otherwise falls back to `ARTSWorker::StaticClass()`.

## Files
- `Source/NyangCraft/RTS/Buildings/RTSBuilding_Barracks.h/.cpp`
- `Source/NyangCraft/RTS/Buildings/RTSBuilding_CommandCenter.h/.cpp`

## How to Use
1. Create `BP_Marine` (child of `ARTSMarine`) and `BP_Worker` (child of `ARTSWorker`).
2. In your `BP_Barracks` defaults, set `MarineClass = BP_Marine`.
3. In your `BP_CommandCenter` defaults, set `WorkerClass = BP_Worker`.
4. Training/production now spawns the blueprint units.

## Date
2025-08-27

