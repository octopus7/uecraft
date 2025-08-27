# REQ018: Move Mineral HUD to Top-Right

## Request Summary (EN)
Change the Mineral HUD position from top-left to top-right with the same offset.

## 원문 요약 (KO)
미네랄 HUD 위치를 좌상단에서 우상단으로 이동(오프셋 유지).

## Changes Implemented
- Update `URTSHUDWidget::NativePaint` to anchor the panel to the top-right.
- Computes `Origin` as `ViewSize.X - PanelSize.X - OffsetX` for right alignment.

## Files
- `Source/NyangCraft/RTS/UI/RTSHUDWidget.cpp`

## Test Steps
1. Run the RTS map.
2. Verify the “Minerals: N” panel renders at the top-right with the configured offset.

## Date
2025-08-27

