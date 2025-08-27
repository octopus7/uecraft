# Repository Guidelines

## Project Structure & Module Organization
- `Source/NyangCraft`: Primary UE5 C++ module. Key files: `NyangCraft.Build.cs` (deps: Core, Engine, InputCore, NavigationSystem, AIModule, Niagara, EnhancedInput), `NyangCraft.cpp/.h`, `NyangCraftGameMode.*`, `NyangCraftCharacter.*`, `NyangCraftPlayerController.*`.
- `Content/`: Game assets and Blueprints (e.g., `/Game/TopDown/...`).
- `Config/`: Project settings. `DefaultEngine.ini` sets `GameDefaultMap`, `GlobalDefaultGameMode=/Script/NyangCraft.NyangCraftGameMode` and platform RHI.
- `NyangCraft.uproject` / `NyangCraft.sln`: Open in Unreal Editor or IDE.
- `Intermediate/`, `Saved/`: Generated; do not commit.

## Build, Test, and Development Commands
- Open Editor: Double‑click `NyangCraft.uproject` or run:
  - Windows: `"<UE_5.5>/Engine/Binaries/Win64/UnrealEditor.exe" NyangCraft.uproject -game`
- Build via UBT (example targets):
  - Windows: `"<UE_5.5>/Engine/Build/BatchFiles/Build.bat" NyangCraftEditor Win64 Development -Project="%CD%\NyangCraft.uproject"`
  - Linux/macOS: use `Build.sh` and `Linux/Mac` targets.
- Regenerate IDE files: Right‑click `.uproject` → Generate Project Files.

## Coding Style & Naming Conventions
- Follow Unreal C++ style: PascalCase types/functions; lowerCamelCase variables; `b` prefix for booleans; type prefixes `U`, `A`, `F`, `I`, `E`.
- Keep headers light with forward declarations; includes in `.cpp`.
- Log with `UE_LOG(LogNyangCraft, ...)`; define categories in module.
- Annotate reflection fields with `UPROPERTY(...)` and `meta=(AllowPrivateAccess="true")` for private Blueprint access.

## Testing Guidelines
- Prefer Unreal Automation/Functional tests. Place C++ tests under `Source/NyangCraft/Tests` using `IMPLEMENT_SIMPLE_AUTOMATION_TEST`.
- Run headless automation:
  - `UnrealEditor-Cmd.exe NyangCraft.uproject -unattended -nop4 -nullrhi -ExecCmds="Automation RunTests Project; Quit"`
- Aim to cover controller input, GameMode rules, and map loads.

## Commit & Pull Request Guidelines
- Use Conventional Commits (e.g., `feat:`, `fix:`, `refactor:`, `test:`). Example: `feat: add Niagara click FX to player controller`.
- PRs include: clear description, linked issues, test steps, and screenshots/video for visual changes. Keep scope focused and update config notes if maps/inputs change.

## Security & Configuration Tips
- Do not commit `Intermediate/` or `Saved/`. Confirm `.gitignore` excludes them.
- Avoid hardcoded asset paths; prefer soft references (`TSoftObjectPtr`) or config.
- When adding modules/plugins, update `*.Build.cs` and verify platform settings in `Config/DefaultEngine.ini`.

## 언어 및 문서화
- 기본 커뮤니케이션: 한국어 사용(이슈/PR 설명, 리뷰 댓글).
- 커밋 메시지: Conventional Commits 영어 태그 권장(`feat:`, `fix:`) + 한글 본문/본문 혼용 가능. 예) `feat: 플레이어 컨트롤러 클릭 이펙트 추가`.
- 코드 주석/식별자: Unreal 관례상 영문 유지 권장(클래스/함수/변수명, API 검색 용이성).
- 문서: 저장소 내 문서는 한국어 우선, 외부 공유용은 영문 병기 가능. 자세한 기준은 `agent.md` 참조.
