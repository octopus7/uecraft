# REQ005: 선택/드래그/명령/채집 루프 디버그 메시지 출력

## Request Summary (EN)
Add on-screen and log debug messages for selection/drag, command issuance, and worker gather/deposit loop to aid verification during development.

Title (Korean): 선택/드래그/명령/채집 루프 확인용 텍스트 메시지 추가

## 변경 요약
- 화면 텍스트(`GEngine->AddOnScreenDebugMessage`)와 로그(`UE_LOG(LogNyangCraft, ...)`) 추가.
- 대상:
  - `ARTSPlayerController`: 드래그 시작/종료, 단일 선택/해제, 드래그 선택 결과 수, 이동 명령, 채집 명령.
  - `ARTSUnit`: `SetSelected` 시 선택/해제 메시지.
  - `ARTSWorker`: 채집 시작, 틱 채집(+n/적재), 반납 이동, 반납(+n), 재채집 등 상태 전환.
  - `ARTSResource_Mineral`: 채굴량/잔여량 로그.
  - `ARTSGameMode`: 미네랄 증가 시 총량 출력.

## 사용 방법
- 에디터에서 플레이 후 마우스 조작:
  - 좌클릭: 단일 선택/해제 메시지 표시.
  - 드래그: 시작/종료/선택된 유닛 수 표시.
  - 우클릭: 이동/채집 명령 메시지 표시.
- 워커가 채집/반납을 수행할 때 단계별 메시지가 화면/로그에 출력.
