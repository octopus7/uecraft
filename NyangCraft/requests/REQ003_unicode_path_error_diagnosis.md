# REQ003: 한글 경로 에러 지속 원인 분석

## Request Summary (EN)
Diagnose why Unicode/Korean path errors persist after deleting offending files and document mitigation: strip quotes, normalize slashes, restart watchers, clear caches, and enforce ASCII filenames for repo paths.

Title (Korean): 파일 삭제 후에도 한글 경로 에러가 지속되는 이유

## 현상
- .NET 예외: `System.ArgumentException: Path fragment '"NyangCraft/requests/\354\232\224...' contains invalid directory separators.`
- 한글 파일을 삭제한 후에도 동일 에러가 반복.

## 원인 분석
- 경로 문자열에 큰따옴표가 포함됨: `"NyangCraft/..."` 형태가 그대로 인자로 전달되어 예외 유발.
- 비ASCII 경로가 잘못 인코딩되어 `\354\232...`와 같이 역슬래시-숫자 시퀀스로 변환됨. 이때 `\`가 디렉터리 구분자로 간주되어 Globbing/경로 정규화에서 오류 발생.
- 프로세스/워처 캐시: 이전 실행에서 수집된 경로가 메모리/캐시에 남아 삭제 후에도 재시도.

## 권장 대응
1) 경로 인자 정리: 외부 툴/스크립트에 경로 전달 시 양끝 큰따옴표 제거, 역슬래시를 슬래시(`/`)로 정규화.
2) 프로세스 재시작: 관련 .NET 프로세스/워처(예: watch, VS/Rider 플러그인) 종료 후 재실행.
3) 캐시 정리(선택): `.vs/`, IDE 캐시, 툴의 임시 캐시 제거 후 재시작.
4) 파일명 정책 적용: ASCII 파일명 유지(`requests/REQ###_slug.md`), 한글 제목은 문서 본문으로.
5) 잔여 참조 점검: 저장소 내 `요청` 키워드/이전 경로 문자열 검색 후 제거.

## 작업 반영
- 요청 파일을 ASCII 이름으로 이관, 한글 파일 삭제.
- `agent.md`에 파일명/경로 정책 추가.
- `requests/INDEX.md`에 매핑 문서화.

