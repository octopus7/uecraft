# REQ014: Bounds.SphereRadius가 너무 작을 때 텍스트로 표시

## Request Summary (EN)
When the target bounds radius is tiny (<10), draw a short-lived debug text above the actor with the computed radius to ensure visibility during tuning.

Title (Korean): 디버그 스피어가 보이지 않을 만큼 반경이 작을 때 머리 위 텍스트 출력

## 변경
- `ARTSWorker::MoveToActor`에 조건 추가: `Bounds.SphereRadius < 10.f`이면 대상 머리 위에 `BoundsR=...` 텍스트를 1초간 표시.
- 위치: 대상 액터 위치 + Z 오프셋(`max(50, R+50)`), 색상: Cyan, 그림자: On, 스케일: 1.2.

## 파일
- `Source/NyangCraft/RTS/Units/RTSWorker.cpp`
