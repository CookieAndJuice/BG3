# Repository Guidelines

## 프로젝트 구조 & 모듈 구성
- `Source/BG3/`: C++ 게임 코드. `Public/` 헤더, `Private/` 소스.
- `Source/BG3/<Domain>/...` 예: `AI`, `Component`, `UI`.
- `Content/`: 에셋(메시, 머티리얼, 블루프린트 등).
- `Config/`: 프로젝트/엔진 설정.
- `Tests/`(선택): Automation/Functional 테스트 소스.

## 빌드 · 테스트 · 실행
- IDE 빌드: Rider/VS에서 `.uproject` 열고 `Build → Rebuild`.
- 명령행(에디터 타겟 예시):
  `"<UE>\Engine\Build\BatchFiles\Build.bat" BG3Editor Win64 Development -Project="<PATH>\BG3.uproject" -WaitMutex`
- 실행: 언리얼 에디터에서 레벨 열고 `Play`.
- 핫리로드: C++ 변경 후 에디터 `Compile` 또는 IDE 빌드.

## 코딩 스타일 & 네이밍
- UE 스타일: `UCLASS`/`USTRUCT`/`UFUNCTION` 매크로, 타입·클래스 PascalCase, 변수 camelCase.
- 속성: `UPROPERTY`/`UPARAM` 메타 유지(가시성, Replication 등 명시).
- 폴더 네임스페이스: `Source/BG3/<Domain>/...`(예: `Component`, `AI`, `UI`).
- 로깅: 모듈별 `DECLARE_LOG_CATEGORY_EXTERN` + `DEFINE_LOG_CATEGORY`; 사용: `UE_LOG(LogMyModule, Log, TEXT("..."));`.
- 들여쓰기 4칸, 한 줄 120자 내.

## 테스트 가이드
- 프레임워크: `AutomationTest.h` 기반 단위/통합 테스트 권장.
- 파일/이름: `*Tests.cpp`, 테스트명 `Project.Feature.Scenario`.
- 실행: 에디터 `Window → Developer Tools → Session Frontend → Automation`.

## Git & PR 가이드
- 브랜치: `feat/`, `fix/`, `chore/` 접두사 사용.
- 커밋 메시지: `type(scope): summary` (예: `fix(skill): null check in AddSkill`).
- PR 요건: 변경 요약, 필요 시 스크린샷/로그, 관련 이슈 링크, 테스트 결과와 영향 범위.

## 보안 & 설정 팁
- 비밀/경로 하드코딩 금지. 민감 값은 `Config/Default*.ini` 기본값으로, 로컬 민감 설정은 `Saved/`에만 저장.
- Git에 커밋 금지: `Binaries/`, `DerivedDataCache/`, `Intermediate/`, `Saved/`.
- 엔진 경로는 환경변수/로컬 설정 사용(개발자별 경로 불일치 방지).

## Agent 전용 메모
- 이 문서는 에이전트와 기여자 공통 가이드입니다. 코드 수정 시 위 구조/네이밍을 준수합니다.
- 자동화 작업은 위 빌드/테스트 경로와 명령을 우선 사용합니다.

