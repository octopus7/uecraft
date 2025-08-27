# REQ002: 드래그 선택 + 워커-미네랄 채집 루프

Title (Korean): 드래그 선택 + 워커-미네랄 채집 루프 구현

## 구현 요약
- 드래그 선택: `ARTSPlayerController`에 드래그 사각형 멀티 선택, 단일 클릭 선택 보완.
- 우클릭 명령: 미네랄 클릭 시 워커에 `IssueGatherOrder`, 그 외 이동 명령.
- 워커 채집 루프: 상태머신으로 채집→반납 반복, 수용량/주기 제어.
- 미네랄 노드: `Harvest`, `IsDepleted` 제공.
- 자원 합산: `ARTSGameMode.AddMinerals`, `GetMinerals`.

## 변경 파일 요약
- `Source/NyangCraft/RTS/Systems/RTSPlayerController.*`
- `Source/NyangCraft/RTS/Units/RTSWorker.*`, `RTSUnit.*`
- `Source/NyangCraft/RTS/Resources/RTSResource_Mineral.*`
- `Source/NyangCraft/RTS/Systems/RTSGameMode.*`

## 테스트 가이드
1) 맵에 `RTSWorker`, `RTSResource_Mineral`, `RTSBuilding_CommandCenter` 배치.
2) 드래그로 워커 선택 → 미네랄 우클릭: 채집→반납 루프 동작 확인.
3) `GetMinerals()`로 누적 확인.

