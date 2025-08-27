# REQ004: RTSBuilding_Base.h 포함 경로 오류 수정

## Request Summary (EN)
Fix C1083 include error for RTSBuilding_Base by adding the module root to Public/Private include paths in Build.cs so headers can be included as "RTS/..." across the module.

Title (Korean): RTSBuilding_Base.h include 오류(C1083) 조치

## 증상
- MSVC 에러: `fatal error C1083: Cannot open include file: 'RTS/Buildings/RTSBuilding_Base.h': No such file or directory`
- 발생 위치: `Source/NyangCraft/RTS/Buildings/RTSBuilding_Base.cpp(1)`

## 원인 추정
- 모듈의 포함(Include) 경로에 모듈 루트(`Source/NyangCraft`)가 노출되지 않아 `#include "RTS/..."` 스타일 경로를 해석하지 못함.
- 일부 환경에서 Public/Private 폴더가 없을 때 루트 자동 포함이 누락될 수 있음.

## 조치
- `Source/NyangCraft/NyangCraft.Build.cs`에 포함 경로 추가:
  - `PublicIncludePaths.Add(ModuleDirectory);`
  - `PrivateIncludePaths.Add(ModuleDirectory);`
- 결과적으로 `#include "RTS/Buildings/RTSBuilding_Base.h"` 경로가 모듈 루트 기준으로 정상 해석됨.

## 후속 안내
- 에디터에서 `.uproject` 우클릭 → Generate Visual Studio project files(필요 시).
- VS에서 Clean → Build 또는 `Build.bat`로 클린 빌드 권장.
