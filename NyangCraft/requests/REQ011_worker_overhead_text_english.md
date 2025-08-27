# REQ011: 워커 머리 위 상태 텍스트 영문화

Title (Korean): 건설로봇(워커) 상태 표기를 영어로 변경

## 변경 내용
- `ARTSWorker::UpdateStatusText()`의 상태 문자열을 한글 → 영어로 변경.
  - Idle, To Resource, Gathering t, To Dropoff, Depositing t
- 적재량 표기는 유지(`x/y`).

## 파일
- `Source/NyangCraft/RTS/Units/RTSWorker.cpp`

