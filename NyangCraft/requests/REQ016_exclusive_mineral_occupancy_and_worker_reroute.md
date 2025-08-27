# REQ016: Exclusive Mineral Occupancy and Worker Reroute

## Request Summary (EN)
Limit each mineral to 1 concurrent worker (exclusive). If a worker issues a gather order on an already-occupied mineral, reroute to a nearby available mineral; if none found, move to wait near the closest mineral.

## 원문 요약 (KO)
미네랄 동시 작업자 수를 1로 제한(독점). 이미 채집 중이면 근처 다른 미네랄로 보내고, 다른 곳도 없으면 가장 가까운 곳에 대기.

## Changes Implemented
- `ARTSResource_Mineral` occupancy state:
  - `TWeakObjectPtr<ARTSWorker> OccupiedBy` added.
  - `TryClaim(Worker)`, `ReleaseClaim(Worker)`, `IsOccupied()`, `GetOccupant()` API 추가.
- `ARTSWorker` gather flow updates:
  - `IssueGatherOrder` attempts `TryClaim` on requested mineral.
  - If occupied, searches alternate minerals near clicked mineral (2,500 units).
  - If alternate found and claimable → proceed; otherwise `WaitNearResource` moves worker to a ring around the mineral and idles.
  - `FinishAllTasks` now clears move/gather/deposit timers and releases any resource claim.
- Debug/Status unchanged aside from existing improvements. Waiting uses `Idle` label currently.

## Files
- `Source/NyangCraft/RTS/Resources/RTSResource_Mineral.h`
- `Source/NyangCraft/RTS/Resources/RTSResource_Mineral.cpp`
- `Source/NyangCraft/RTS/Units/RTSWorker.h`
- `Source/NyangCraft/RTS/Units/RTSWorker.cpp`

## Test Steps
1. Spawn two workers and one mineral.
2. Order Worker A to gather → should claim and move/gather.
3. Order Worker B to the same mineral → should either:
   - Reroute to a nearby free mineral (if present), or
   - Move near the original mineral and wait (Idle), not gathering.
4. Deplete or release the mineral (stop A) and reissue order to B → should be able to claim now.

## Notes / Follow-ups
- Current design keeps claim for the worker through deposit trips. If you prefer releasing claim while depositing, we can change to allow faster worker turnover.
- We can add periodic retry while waiting to auto-claim when the mineral frees up.

## Date
2025-08-27

