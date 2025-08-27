# REQ010: C2181 illegal else 수정 + 상태 텍스트 컴파일 정리

## Request Summary (EN)
Resolve an `illegal else without matching if` compiler error by fixing block structure and moving UpdateStatusText calls outside conditional branches; also tidy private UPROPERTY access for status text.

Title (Korean): 불일치 else 문법 오류(C2181) 수정 및 워커 상태 텍스트 컴파일 정리

## 증상
- 컴파일 에러: `illegal else without matching if` (RTSWorker.cpp:92 근처)
- 원인: if/else 블록 사이에 `UpdateStatusText();` 호출이 잘못 삽입되어 블록 경계가 깨짐.

## 조치
- `RTSWorker.cpp`에서 `UpdateStatusText();` 호출 위치를 if/else 블록 바깥(두 분기 이후)으로 이동.
- 동일 파일에 누락 가능성이 있는 중괄호 정렬 확인 및 정리.
- 워커 머리 위 상태 텍스트 관련 private UPROPERTY에 `meta=(AllowPrivateAccess="true")` 추가(빌드 규칙 위반 경고 방지).

## 참고
- 관련 파일: `Source/NyangCraft/RTS/Units/RTSWorker.h/.cpp`
- 기능: 상태 텍스트 표시(대기/이동/채집/반납 + 적재량), 채집 루프 튜닝 유지.
