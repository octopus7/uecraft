# REQ001: RTS 기본 기능 구현 요청 기록

## Request Summary (EN)
Build core RTS gameplay: production, gathering, movement, and construction by refactoring the TopDown template into an RTS architecture (units, buildings, resources, systems, and UI). Focus on Command Center, Supply Depot, Barracks, Workers, Marines, and Minerals.

Title (Korean): RTS 기본 기능 구현 요청

## 요청 요약
- 현재 프로젝트: 언리얼 엔진 TopDown 템플릿 기반.
- 목표: 스타크래프트 유사 RTS로 확장.
- 1차 범위: 생산, 채집, 이동, 건설만 구현.
- 대상 요소: 미네랄, 커맨드센터, 건설로봇(SCV), 보급창, 병영, 마린.
- 기존 코드: 불필요하면 삭제 가능.
- 운영 정책: 모든 요청/응답은 `requests/REQ###_slug.md`로 기록.

## 원문 요청(요약)
한국어 원문은 초기 커밋 메시지를 참조. 파일명은 ASCII로 관리합니다.

## 에이전트 응답 및 제안
- 제안 아키텍처(초안):
  - 디렉터리: `Source/NyangCraft/RTS/{Systems,Units,Buildings,Resources,UI}`
  - 리소스/유닛/건물: Mineral, SCV, Marine, CommandCenter, SupplyDepot, Barracks
  - 시스템: Selection/Command, BuildSystem, ProductionQueue, Resource/SupplySystem
- 단계 계획: RTS 컨트롤러/카메라 → 미네랄/자원 UI → SCV 채집 → 건설 → 생산 → 보급 → HUD
- 템플릿 정리: TopDown 캐릭터 중심 로직은 RTS 구조로 대체.

## 진행 로그(발췌)
- RTS 골격 추가: `RTSGameMode`, `RTSPlayerController`, `RTSCameraPawn`, `RTSUnit/Worker/Marine`, `RTSBuilding_*`, `RTSResource_Mineral`
- `DefaultEngine.ini` → `GlobalDefaultGameMode="/Script/NyangCraft.RTSGameMode"`

