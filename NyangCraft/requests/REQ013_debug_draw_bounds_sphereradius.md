# REQ013: 대상 액터 Root Primitive Bounds.SphereRadius 디버그 스피어 표시

Title (Korean): 목표 액터 경계 반경 시각화(1초 유지)

## 변경
- `ARTSWorker::MoveToActor`에서 대상 액터 Root Primitive의 `Bounds.SphereRadius`를 디버그 스피어로 표시.
- 색상: Cyan, 세그먼트: 24, 지속: 1.0초, 두께: 1.5.
- Shipping/Test 빌드에서는 표시하지 않음.

## 파일
- `Source/NyangCraft/RTS/Units/RTSWorker.cpp`

