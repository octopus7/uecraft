# REQ006: 채집 1회만 수행되는 현상 진단/개선

## Request Summary (EN)
Investigate why the gather loop runs only once and fix pathing/target issues: project to navmesh for reachable points, add clear warnings, and ensure dropoff exists to keep the loop running.

Title (Korean): 채집이 1회만 되고 반복되지 않는 문제 분석/조치

## 현상
- 미네랄 채집 명령 메시지는 출력되나, 채집이 1회만 수행되고 루프가 반복되지 않음.

## 원인 후보
1) 드랍오프(커맨드센터) 미배치 → 가득 채운 뒤 반납 대상 없음으로 Idle 전환.
2) 자원/건물 근처 네비메시 부재 → 복귀/접근 경로 없음으로 이동 정체.

## 코드 조치(적용됨)
- 워커 이동 보강: 목표 위치를 네비메시 근처 도달 가능한 점으로 투영(`ProjectPointToNavigation`).
  - `RTSWorker`: `MoveToLocationOnNav`, `FindReachablePointNear` 추가, 이동 결과 로그/화면 메시지.
- 루프 디버그 메시지 강화:
  - 드랍오프 대상 없음, 자원 근처 도달 불가 시 화면/로그 경고 출력.

## 사용 가이드
- 맵에 `RTSBuilding_CommandCenter`를 최소 1개 배치.
- 미네랄 StaticMesh: 필요 시 Collision 완화 또는 "Can Ever Affect Navigation" 해제.
- `RTSWorker.AcceptanceRadius` 150~200 권장(블루프린트 인스턴스에서 조정 가능).
