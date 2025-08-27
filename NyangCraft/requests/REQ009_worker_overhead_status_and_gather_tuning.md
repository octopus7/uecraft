# REQ009: 건설로봇(워커) 상태 표시 + 채집 접근 튜닝

## Request Summary (EN)
Add a TextRenderComponent above workers to show state and carried amount; raise AcceptanceRadius and update status on transitions to reduce stops near resources and improve gather start.

Title (Korean): 워커 머리 위 상태 텍스트와 채집 정지 문제 개선

## 구현
- 워커 머리 위 상태 텍스트: `UTextRenderComponent` 추가(`StatusText`).
  - 상태 요약 출력: 대기, 이동-자원, 채집 t, 이동-반납, 반납 t + 적재량(x/y).
  - 상태 전환 및 타이머 진행 시 갱신.
- 채집 접근 튜닝:
  - `AcceptanceRadius` 기본값을 250으로 상향(자원/장애물 주변 네비 투영 시 도달 판정 완화).
  - 상태 전환 시점마다 상태 텍스트 갱신.

## 파일
- `Source/NyangCraft/RTS/Units/RTSWorker.h/.cpp`

## 테스트
1) SCV를 배치하고 미네랄 우클릭.
2) 워커 머리 위에 현재 상태와 적재량 표시됨을 확인.
3) 자원 근처에서 멈춘 경우에도 AcceptanceRadius 확대로 채집 전환이 되는지 확인.
