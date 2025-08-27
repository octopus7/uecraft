# REQ015: Overhaul Worker Movement and Gathering

## Request Summary (EN)
Reimplement movement and gathering for RTS workers from scratch, replacing the previous tick-based FSM. Add robust arrival handling, gathering/deposit loops, and clear debug visuals. Ensure status text faces the camera and is legible.

## 원문 요약 (KO)
이동 및 채집 로직에 문제가 있어 기존 로직을 제거하고 새로 구현. 이동 도착 판정 개선, 채집/반납 루프 안정화, 디버그 스피어 표시, 머리 위 상태 텍스트 빌보드 처리 및 가독성 향상.

## Changes Implemented
- Event/timer-driven worker flow (no per-tick FSM): Move → Arrive → Gather timer → Full/Depleted → Move to Dropoff → Deposit → Repeat.
- Arrival handling via polling with both actor-bound radius and projected reachable location tolerance.
- Tiny target bounds handling: if target `Bounds.SphereRadius < 10`, treat as 100 for arrival logic and debug draw.
- Debug draw while moving:
  - Target bounds: Pink sphere (Red if tiny), at target location.
  - Worker acceptance radius: Green sphere centered on the worker.
- Status text:
  - Doubled size for readability.
  - Billboard to camera on all axes (each Tick).
- Movement targeting:
  - Prefer `FindReachablePointNear` + `MoveToLocation`; fallback to `MoveToActor`.
  - On failure, tasks cleanly stop.

## Files
- `Source/NyangCraft/RTS/Units/RTSWorker.h`
- `Source/NyangCraft/RTS/Units/RTSWorker.cpp`

## Test Steps
1. Build and run the project.
2. Select a worker and right-click a mineral.
3. Observe:
   - Worker moves to a reachable point near the mineral.
   - Pink/Red target bounds and Green worker acceptance radius debug spheres draw while moving.
   - Upon arrival, state switches to Gathering; on full/depleted, worker moves to dropoff and deposits, then resumes if resource remains.
   - Overhead status text faces the camera and is larger.

## Notes / Follow-ups
- If arrival still stalls on certain maps, consider increasing `AcceptanceRadius` or expanding the navmesh around resource actors.
- We can optionally draw the chosen reachable point in Blue for further diagnostics.

## Date
2025-08-27

