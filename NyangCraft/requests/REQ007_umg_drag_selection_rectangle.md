# REQ007: UMG 드래그 선택 영역 표시 구현

## Request Summary (EN)
Implement a UMG selection rectangle that renders while dragging, with controller integration to update start/end and show/hide at begin/end of drag.

Title (Korean): UMG로 드래그 박스 시각화 추가

## 구현 내용
- `URTSSelectionWidget`(UUserWidget)
  - 드래그 시작/업데이트/종료 시 직사각형 외곽선을 그리는 위젯.
  - `SetSelectionRect(bSelecting, Start, End)`로 상태/좌표 갱신.
  - `NativeOnPaint`에서 `UWidgetBlueprintLibrary::DrawLine`으로 4변을 그림.
- `ARTSPlayerController`
  - `BeginPlay`에서 위젯 생성/뷰포트 추가.
  - `BeginSelection/UpdateSelectionDrag/EndSelection`에서 위젯에 좌표 전달.
- 빌드 설정: `NyangCraft.Build.cs`에 `UMG`, `Slate`, `SlateCore` 추가.

## 테스트
1) 플레이 시작 후 좌클릭 드래그 → 화면에 녹색 외곽선 사각형 표시.
2) 드래그 해제 시 사각형 사라지고 선택 결과 메시지 출력.
