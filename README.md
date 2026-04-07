# BG3 - Baldur's Gate 3 턴제 RPG

**Unreal Engine 5.6**으로 구현된 턴제 전술 RPG입니다. Baldur's Gate 3의 전투 시스템과 D&D 5판의 행동력 시스템(Action/Bonus Action/Reaction/Movement)을 C++ 기반으로 재현합니다.

## 시연 영상
[![BG3 게임플레이 영상](https://img.shields.io/badge/YouTube-시연%20영상%20보기-red?style=for-the-badge&logo=youtube)](https://drive.google.com/file/d/1vF0U6NqM_qRvn9XLRSNhpf9MIGUOkbuY/view?usp=sharing)

[📹 시연 영상 보기](https://drive.google.com/file/d/1vF0U6NqM_qRvn9XLRSNhpf9MIGUOkbuY/view?usp=sharing)

## 목차
- [주요 특징](#주요-특징)
- [기술 스택](#기술-스택)
- [프로젝트 구조](#프로젝트-구조)
- [핵심 시스템](#핵심-시스템)
- [실행 방법](#실행-방법)
- [개발 환경](#개발-환경)
- [게임플레이](#게임플레이)
- [현재 상태](#현재-상태)

## 주요 특징

### 전투 시스템
- **턴제 전투 루프**: D&D 5판 Initiative 시스템 기반 턴 순서 관리
- **행동력 시스템**: Action, Bonus Action, Reaction, Movement 4가지 행동 예산 관리
- **스킬 실행 파이프라인**: 예약(Reserve) → 타게팅(Targeting) → 확정(Commit) → 실행(Execute) 4단계 워크플로우
- **타게팅 시스템**: 단일/다중/영역/자가 타게팅 지원, 사거리 및 최대 대상 수 검증
- **데미지 계산**: D&D 주사위 시스템 (NdM) 기반 데미지 롤

### UI 시스템
- **전투 HUD**:
  - 행동 슬롯 그리드 (스킬 아이콘, 쿨다운, 사용 가능 여부)
  - 턴 순서 표시 (캐릭터 초상화 + HP 바)
  - 실시간 행동력 및 이동 거리 표시
- **전투 연출**: 승리/패배 Fade-out 애니메이션, 포트레이트 전환
- **위젯 컨트롤러 패턴**: UI 로직과 데이터 분리

### AI 시스템
- **적 AI FSM**: Idle → Plan → Execute 상태 머신
- **자동 행동**: 대상 선택 (최근접/최저체력), 스킬 선택, 이동 및 공격
- **턴 자동 종료**: AI 행동 완료 시 자동으로 다음 턴 진행

### 연출 및 사운드
- 배경음악 및 효과음 (전투, 메뉴, 스킬)
- 애니메이션 + Niagara 이펙트 통합
- AnimNotify 기반 타이밍 동기화

## 기술 스택

### 엔진 및 언어
- **Unreal Engine 5.6**
- **C++ 모듈**: `BG3` (Runtime)
- **블루프린트**: UI 위젯, 에셋 설정

### Unreal 시스템
| 시스템 | 사용처 |
|--------|--------|
| **UMG (Unreal Motion Graphics)** | 전투 UI, 메인 메뉴 |
| **Enhanced Input System** | 마우스 클릭, 확정/취소 입력 |
| **Navigation System (NavMesh)** | AI 및 플레이어 이동 경로 계산 |
| **AIModule** | 적 AI FSM |
| **Niagara** | 스킬 이펙트 (Fireball 등) |
| **AnimNotify** | 애니메이션 히트 타이밍 |
| **Gameplay Tags** | 데미지 타입 분류 (`Damage.Physical`) |
| **Data Assets** | 스킬 정의, 캐릭터 초기 정보 |
| **Subsystems (WorldSubsystem)** | 전역 게임 상태 관리 |

## 프로젝트 구조

```
E:\UnrealProject\BG3/
├── Source/BG3/                    # C++ 소스 코드
│   ├── Public/                    # 헤더 파일 (54개)
│   │   ├── Character/             # 캐릭터 (Base, Player, Enemy)
│   │   ├── Component/             # 게임 컴포넌트 (SkillBook, Stats, AI)
│   │   ├── Game/                  # 게임 매니저 및 Subsystems
│   │   │   └── Skill/             # 스킬 실행 (Tasks, Projectile)
│   │   ├── Data/                  # 데이터 에셋 (Skill, Stats)
│   │   ├── UI/                    # UI 위젯 및 컨트롤러
│   │   ├── Controller/            # 플레이어 컨트롤러
│   │   ├── Manager/               # 카메라, 주사위 매니저
│   │   └── Interface/             # 인터페이스 (ActionBudgetProvider)
│   └── Private/                   # 구현 파일 (56개 .cpp)
├── Config/                        # 프로젝트 설정
│   ├── DefaultEngine.ini
│   ├── DefaultGame.ini
│   ├── DefaultGameplayTags.ini
│   └── DefaultInput.ini
├── Content/                       # 게임 에셋 (블루프린트, 메시, 애니메이션)
├── BG3.uproject                   # 프로젝트 파일
└── BG3.sln                        # Visual Studio 솔루션
```

## 핵심 시스템

### 1. 스킬 시스템

#### 스킬 실행 파이프라인
```
플레이어 스킬 버튼 클릭
    ↓
1. Reserve (예약)
   - SkillBookComponent::ReserveUse()
   - 행동력 소비 검증 (IActionBudgetProvider)
   - 예약 상태 설정
    ↓
2. Targeting (타게팅)
   - USkillExecutionSubsystem::RequestCast()
   - 대상 선택 (마우스 클릭)
   - 사거리, 최대 대상 수 검증
    ↓
3. Confirm (확정)
   - USkillExecutionSubsystem::ConfirmAndExecute()
   - Task 배열 생성 (USkillCastExecutor)
    ↓
4. Execute (실행)
   - Task 순차 실행 (FaceTarget → MoveTo → PlayMontage → Damage)
   - 데미지 적용 (주사위 롤)
    ↓
5. Finalize (완료)
   - 쿨다운 적용
   - 상태 초기화
```

#### Skill Task 시스템
스킬 실행을 작은 비동기 단계로 분할하여 순차 오케스트레이션:

| Task | 역할 |
|------|------|
| **SkillTaskFaceTarget** | 대상 방향으로 캐릭터 회전 |
| **SkillTaskMoveTo** | NavMesh 경로 이동 (사거리 진입) |
| **SkillTaskPlayMontage** | 공격 애니메이션 재생 (AnimNotify 대기) |
| **SkillTaskSpawnProjectile** | 발사체 생성 및 발사 |
| **SkillTaskSpawnSystem** | Niagara 이펙트 생성 |
| **SkillTaskEndTurn** | 턴 종료 처리 |

**예시 - 근접 공격 실행 흐름**:
```cpp
Task 1: FaceTarget      → 대상 방향
Task 2: MoveTo          → 사거리 진입
Task 3: PlayMontage     → 공격 애니메이션
        [AnimNotify "Hit" 발생]
        ↓
        데미지 적용 (주사위 롤)
Task 4: EndTurn         → 턴 종료
```

### 2. 행동력 시스템 (D&D 5판)

| 행동 유형 | 턴당 개수 | 비용 | 예시 |
|---------|---------|------|------|
| **Action** | 1 | 1 | 공격, 주문, 회피 |
| **Bonus Action** | 1 | 1 | 보조 주문, 특수 능력 |
| **Reaction** | 1 | 1 | 기회 공격, 반응 마법 |
| **Movement** | ~10칸 | 750 유닛 | 이동 거리 |

**구현 인터페이스**:
```cpp
class IActionBudgetProvider
{
    virtual bool CanSpendActionSlot(EActionCost Cost) = 0;
    virtual void SpendActionSlot(EActionCost Cost) = 0;
    virtual void RefundActionSlot(EActionCost Cost) = 0;
    virtual void BeginTurnReset() = 0;  // 턴 시작 시 초기화
};
```

**주요 클래스**:
- `ABaseCharacter`: 모든 캐릭터의 기본 클래스, 행동력 및 스킬북 관리
- `USkillBookComponent`: 스킬 목록 및 런타임 상태 (쿨다운, 예약)
- `UCharacterStatsComponent`: HP, Mana, 이동 거리

### 3. 턴 시스템

#### 전투 초기화 및 턴 순환
```
UBG3GameManageSubsystem::InitializeGame()
    ↓
1. 캐릭터 생성
   - SpawnPlayers() → 플레이어 파티
   - SpawnEnemies() → 적 그룹
    ↓
2. Initiative 계산
   - CalcInitiative() = 1d20 + DEX 수정자
   - CombatPawns[] 정렬 (내림차순)
    ↓
3. 턴 루프
   BeginNextTurn()
   ├─ CurrentCharacter.BeginTurnReset()
   │   └─ 행동력 초기화, 쿨다운 -1
   ├─ 플레이어 턴: 입력 대기
   │   └─ 스킬 선택 → 타게팅 → 실행 → 턴 종료 버튼
   ├─ 적 턴: AI 자동 실행
   │   └─ Plan (대상 선택) → Execute (스킬 실행) → 자동 턴 종료
   └─ DecideWhoWin() → 전투 종료 판정
```

**승패 판정**:
- 모든 적이 사망 → 플레이어 승리
- 모든 플레이어가 사망 → 적 승리
- UI Fade-out 애니메이션 재생

### 4. AI 시스템 (USimpleEnemyFSMComponent)

**상태 머신**:
```cpp
enum class ECharacterState : uint8
{
    None,     // 초기화
    Idle,     // 턴 대기
    Plan,     // 행동 계획 (대상/스킬 선택)
    Execute   // 행동 실행 (Task 순차 실행)
};
```

**AI 로직**:
1. **StartMyTurn()**: Idle → Plan
2. **Plan 단계**:
   - `FindNearestTarget()`: 최근접 대상 선택
   - `FindLowestHPTarget()`: 최저체력 대상 선택
   - 스킬 선택 (스킬 ID: 1, 20, 85, 86, 888)
3. **Execute 단계**:
   - `DoAction(skillID)`: 스킬 실행 요청
   - Task 순차 실행 (이동 → 회전 → 공격)
4. **EndMyTurn()**: 턴 종료 신호

### 5. UI 시스템

#### 위젯 목록

| Widget | 역할 |
|--------|------|
| **OverlayWidget** | 메인 전투 HUD (행동 슬롯 + 턴 정보) |
| **CombatActionPanel** | 행동 슬롯 그리드 (스킬 버튼) |
| **ActionSlotEntry** | 개별 스킬 버튼 (아이콘, 쿨다운, 사용 가능 여부) |
| **PlayerHpFrameWidget** | 플레이어 HP 바 |
| **CharacterTurnHpWidget** | 턴 순서 표시 (초상화 + HP) |
| **TurnOrderFrameWidget** | 턴 순서 목록 |
| **TurnEndWidget** | 턴 종료 버튼 |
| **PlayerPortraitWidget** | 캐릭터 초상화 |
| **PressEnterKeyWidget** | 전투 결과 화면 (Enter 대기) |
| **MainMenuWidget** | 메인 메뉴 |

#### 위젯 컨트롤러 패턴
```cpp
UOverlayWidgetController
    ├─ Initialize(Character, PlayerController)
    ├─ RefreshSlots()  // 행동 슬롯 갱신
    └─ BuildAndBroadcast()  // UI 데이터 브로드캐스트

// 이벤트 델리게이트
FOnActionSlotsUpdated OnActionSlotsUpdated;
FOnStatChangedUI OnHealthChanged;
FOnMoveDistanceChangedUI OnMoveDistanceChanged;
FOnFadeOutAnimationStart OnFadeOutAnimationStart;
```

## 실행 방법

### 1. 에디터에서 실행
1. `BG3.uproject`를 Unreal Engine 5.6 에디터에서 엽니다.
2. `Content/Maps/Dream_Island` 레벨을 엽니다.
3. **Play** 버튼을 눌러 전투를 시작합니다.

### 2. C++ 코드 수정
1. Visual Studio에서 `BG3.sln`을 엽니다.
2. 코드 수정 후 빌드 (`Ctrl+Shift+B`).
3. 에디터에서 핫 리로드 또는 에디터 재시작.

### 3. 메인 메뉴에서 시작
1. `Content/Maps/MainMenuMap` 레벨 실행.
2. 메인 메뉴에서 **Start Game** 클릭.
3. 전투 레벨 (`Dream_Island`)로 전환.

## 개발 환경

### 필수 요구사항
- **Unreal Engine**: 5.6 이상
- **IDE**: Visual Studio 2022 (권장)
- **운영체제**: Windows 10/11
- **.NET Framework**: 4.6.2 이상

### 프로젝트 설정
- **게임 기본 맵**: `MainMenuMap`
- **에디터 시작 맵**: `Dream_Island`
- **해상도**: 1920x1080 (DesignScreenSize)
- **UI 스케일 규칙**: ShortestSide
- **타겟 하드웨어**: Desktop, Maximum Graphics

## 게임플레이

### 조작 방법
- **마우스 좌클릭**: 이동 또는 대상 선택
- **마우스 우클릭**: 스킬 실행 확정 (Confirm)
- **ESC**: 스킬 취소 (Cancel)
- **스킬 버튼 클릭**: 스킬 예약 및 타게팅 시작
- **Turn End 버튼**: 현재 턴 종료

### 플레이 흐름
1. **게임 시작**: Initiative 롤 → 턴 순서 결정
2. **플레이어 턴**:
   - 이동: 마우스 클릭 (NavMesh 경로 표시)
   - 스킬 사용:
     1. 스킬 버튼 클릭 (행동력 소비)
     2. 대상 선택 (마우스 클릭)
     3. 우클릭으로 확정 → Task 실행
   - 턴 종료: **Turn End** 버튼 클릭
3. **적 턴**: AI 자동 실행 (대상 선택 → 이동 → 공격)
4. **전투 종료**: 승리/패배 화면 → Enter로 메인 메뉴

### 스킬 목록 (예시)
| 스킬 ID | 이름 | 타입 | 사거리 | 데미지 |
|---------|------|------|--------|--------|
| 1 | Melee Attack | 근접 | 3m | 1d8 + STR |
| 20 | Ranged Attack | 원거리 | 18m | 1d10 + DEX |
| 85 | Fireball | 마법 (영역) | 30m | 8d6 (Fire) |
| 86 | Magic Missile | 마법 (발사체) | 36m | 1d4+1 (Force) |
| 888 | Dash | 비전투 | - | 이동 거리 2배 |

## 현재 상태

### 완성된 기능
- ✅ **전투 시스템**: 턴제 전투 루프, 행동력 관리
- ✅ **스킬 실행**: 예약→타게팅→커밋→Task 실행
- ✅ **UI 시스템**: 행동 슬롯, 턴 순서, HP 바, 전투 결과
- ✅ **이동 시스템**: NavMesh 기반, 거리 제한, 실시간 표시
- ✅ **적 AI**: 간단한 FSM (Plan → Execute)
- ✅ **연출**: Fade-out 애니메이션, 포트레이트 전환
- ✅ **사운드**: 배경음악, 효과음 (전투, 스킬)

### 향후 개선 과제
- ⬜ **데이터 에셋 수치화**: 모든 스킬 파라미터를 에디터에서 편집 가능하게
- ⬜ **블루프린트 자동화**: 스킬 정의 프로세스 간소화
- ⬜ **UI 바인딩 정리**: Widget-Controller 간 느슨한 결합 강화
- ⬜ **더 복잡한 AI**: 거리 계산, 대형 몹, 조건부 행동
- ⬜ **추가 스킬**: 버프/디버프, 치유, 영역 지속 효과
- ⬜ **네트워크 동기화**: 멀티플레이어 지원 (선택)

## 참고 문서
- **설계 문서**: `Docs/Design_SkillExecutionSubsystem.md`
- **커밋 요약**: `Lauren_commit_summary_2025-09-16_19.txt`

## 라이선스
이 프로젝트는 학습 목적으로 제작되었습니다.

## 문의
프로젝트 관련 문의는 GitHub Issues를 통해 제출해주세요.

---

**프로젝트 목표**: C++ 기반의 빠른 전투 루프 검증, 스킬 예약→타게팅→커밋 파이프라인, 턴제 UI 통합, AI 적 턴 자동화, 전투 연출 및 사운드 완성도 확보
