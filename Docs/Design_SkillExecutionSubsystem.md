# SkillExecutionSubsystem 설계 가이드 및 대화 정리

> 다음 내용은 이전 대화에서 논의한 내용을 그대로(맥락 유지) 문서 형태로 정리한 것입니다. 표현은 가독성을 위해 소제목/목록으로만 최소 편집하였고, 제안과 결론은 대화 내용과 동일합니다.

## 현재 구조 요약
- 코어/게임
  - `ABG3GameMode`: 주사위 매니저 보유, 간단한 스탯 보정 계산.
  - `UBG3GameManageSubsystem`: 플레이어/적 스폰, 전투 참여자 목록, 턴/라운드 관리 자리.
  - `ABG3GameState`: 라운드/턴/인원 집계용 기초 상태 보유(아직 로직 없음).
- 캐릭터/컴포넌트/데이터
  - `ABaseCharacter` + `IActionBudgetProvider`: 액션/보너스/리액션 소모/환불 인터페이스 구현.
  - `USkillBookComponent`: 보유 스킬, 쿨다운/충전/예약/커밋·취소 관리. UI에 `OnCooldownChanged`, `OnUsabilityChanged` 브로드캐스트.
  - 데이터: `USkillDefinition`(메타/코스트/타게팅/피해), `USkillSet`, `UCharacterArchetype`, `UInitialCharacterInfo`.
- UI
  - 위젯: `UOverlayWidget` → `UCombatActionPanel` → `UActionSlotEntry`.
  - 컨트롤러: `UOverlayWidgetController`(권장, 델리게이트로 UI 갱신). `UCombatActionWidgetController`는 거의 동일 로직으로 중복.
- 기타
  - 카메라: `ABG3GameCamera` Pawn을 스폰/포제션하여 이동/줌/회전 처리.

## 기능 유형별 권장 구조
- 전투 스킬 실행(타게팅/명중/피해/쿨다운)
  - 서브시스템: `USkillExecutionSubsystem`(UWorldSubsystem) 신설. 시작-타게팅-해결-효과-커밋 파이프라인 담당.
  - 흐름: UI(`UOverlayWidgetController::RequestUseSkill`) → `USkillExecutionSubsystem::StartCasting(ABaseCharacter*, USkillDefinition*)` → 타게팅(커서/선택) → 해결(d20/세이브/오토힛: `UBG3DiceManager` 사용) → 성공 시 `USkillBookComponent::CommitUse(… CurrentRound)`.
  - 데이터: `USkillDefinition::Resolution`, `Targeting` 활용. 필요 시 히트/세이브 규칙 보조 구조체 추가.
  - UI 연동: `USkillBookComponent` 델리게이트 유지. 타게팅 중 상태/취소는 별도 델리게이트(`OnTargetingStarted/Ended`) 추가.
- 턴/라운드 시스템(이니셔티브/턴 순환/턴 시작·종료)
  - `UBG3GameManageSubsystem` 확장: 정렬, 인덱스 순환, 턴/라운드 델리게이트.
  - 턴 시작 시 `SkillBook->OnOwnerTurnStart()` 호출로 쿨다운/예약 상태 갱신.
- 액션 예산 고도화: 필요 시 `UActionBudgetComponent`로 분리 권장.
- UI 확장: 컨트롤러 단일화(`UOverlayWidgetController`), `FActionSlotView` 확장.

## 서브시스템 vs 컴포넌트
- 서브시스템(추천)
  - 장점: 월드 단일 진실원천, 교차 액터 로직에 적합, 테스트/UI 연동 단순, 중복/소유권 충돌 최소화.
  - 단점: 네트워크 복제 고려 필요하지만(이번 프로젝트는 싱글이므로 무관), 일반적으로 GameState/RPC로 해결.
- 컴포넌트 방식(CombatComponent)
  - 장점: 캐릭터 개별 능력/애니/모션과 밀접, 소유자 모델 자연스러움.
  - 단점: 교차 액터 파이프라인 분산 위험, 로직 중복, UI/카메라/GM 결합 증가.
- 결론: 이미 `USkillBookComponent`가 “상태”를 담당하므로, “실행”은 월드 단일 오케스트레이터로 분리하는 것이 유지보수/확장에 유리.

## 두 개 이상의 월드 서브시스템 사용 여부
- 가능: `UBG3GameManageSubsystem`(스폰/턴) + `USkillExecutionSubsystem`(스킬 실행) 병행 운용 권장.
- 선택 기준: 월드 스코프 상태/흐름은 `UWorldSubsystem`, 맵 전환 유지가 필요하면 `UGameInstanceSubsystem`.

## 멀티플레이 고려가 없어도 서브시스템이 유리한가?
- 예. 싱글플레이여도 전투 실행은 월드 서브시스템으로 분리하는 편이 가장 깔끔함.
- 이유: 단일 오케스트레이터, 교차 액터 로직 집약, 상태/프로세스 분리, UI 연동 단순화.

## USkillExecutionSubsystem 설계/구현 가이드

### 설계 목표
- 단일 오케스트레이터: “검증 → 타게팅 → 판정 → 효과 → 커밋”을 중앙에서 일관 관리
- 역할 분리: 상태(`USkillBookComponent`), 절차(`USkillExecutionSubsystem`)
- UI/입력/턴과 느슨 결합: 델리게이트 기반 신호 교환

### 책임 분리
- 서브시스템(실행 절차): 사용 요청 수락/거절, 타게팅/확인/취소, 판정, 효과 적용, 커밋 정책
- 스킬북(상태): 사용 가능 여부, 예약/커밋/취소, 쿨다운/이벤트 브로드캐스트

### 핵심 타입
- `ECastState`: `Idle`, `Targeting`, `Resolving`, `Applying`, `Completed`, `Canceled`
- `FCurrentCast`
  - `TWeakObjectPtr<ABaseCharacter> Caster`
  - `TWeakObjectPtr<USkillDefinition> Skill`
  - `TArray<TWeakObjectPtr<AActor>> Targets`
  - `FVector TargetPoint`, `bool bHasTargetPoint`
  - `bool bReserved`, `int32 StartRound`, `FGuid CastId`
  - `UPROPERTY() USkillExecutor* Executor`(선택)
- 의존성 캐시(선택): `ABG3GameMode`, `UBG3GameManageSubsystem`
- 정책/설정(선택): `bCancelOnTurnEnd`, `TargetTraceChannel`

### 퍼블릭 API(권장)
- `bool RequestCast(ABaseCharacter* Caster, USkillDefinition* Skill)`
- `void CancelCast()`
- `void SetTargets(const TArray<AActor*>& InTargets)` / `void SetTargetPoint(const FVector& InPoint)`
- `bool ConfirmAndExecute(int32 CurrentRound)`
- `bool IsBusy() const`, `ECastState GetState() const`
- 델리게이트: `OnCastingStarted`, `OnTargetsUpdated`, `OnSkillResolved`, `OnCastingCanceled`, `OnCastingFailed`

### RequestCast 절차(핵심)
1) 상태 가드: `State == Idle` 확인(아니면 실패 브로드캐스트)
2) 인자 검증: `Caster`, `Skill` 유효성 체크
3) 스킬북 획득/검증: 소유/사용가능(`GetUsability`)
4) 예약 시도: `SkillBook->ReserveUse(Skill)` 실패 시 거절
5) 현재 캐스트 설정: 캐스터/스킬/캐스트ID/예약 상태 저장, 필요 시 Executor 생성·초기화
6) 상태 전환: `State = Targeting`, `OnCastingStarted` 브로드캐스트

간단 예시(요지):
```cpp
bool USkillExecutionSubsystem::RequestCast(ABaseCharacter* Caster, USkillDefinition* Skill)
{
    if (State != ECastState::Idle) { OnCastingFailed.Broadcast("Busy"); return false; }
    if (!IsValid(Caster) || !IsValid(Skill)) { OnCastingFailed.Broadcast("InvalidArgs"); return false; }

    USkillBookComponent* Book = Caster->FindComponentByClass<USkillBookComponent>();
    if (!Book) { OnCastingFailed.Broadcast("NoSkillBook"); return false; }

    bool bUsable = false; Book->GetUsability(Skill, bUsable);
    if (!bUsable) { OnCastingFailed.Broadcast("NotUsable"); return false; }

    if (!Book->ReserveUse(Skill)) { OnCastingFailed.Broadcast("ReserveFailed"); return false; }

    Current = FCurrentCast{};
    Current.Caster = Caster;
    Current.Skill  = Skill;
    Current.CastId = FGuid::NewGuid();
    Current.bReserved = true;

    if (Skill->ExecutorClass)
    {
        Current.Executor = NewObject<USkillExecutor>(this, Skill->ExecutorClass);
        if (Current.Executor) { Current.Executor->Initialize(this, Caster, Skill); }
    }

    State = ECastState::Targeting;
    OnCastingStarted.Broadcast(Caster, Skill);
    return true;
}
```

### 내부 흐름(확정까지)
- Targeting: 입력 수집, `RangeMeters`/LoS/`MaxTargets` 검증, 미리보기 이벤트
- Confirm: 거리/LoS 재검증 → `Resolution`에 따라 판정(`ToHit`/`Save`/`AutoHit`) → 피해 롤(`FDice`) → 적용(`ApplyDamage` 등) → `SkillBook->CommitUse(Skill, CurrentRound)` → 결과 브로드캐스트 → 정리
- Cancel: 예약 환불(`CancelReservation`), 상태 정리, 이벤트 브로드캐스트

### 판정/데미지 구현 팁
- 주사위: `GM->Dice->RollDice(N, Sides)`
- 보정치: 임시 값으로 시작 후 능력치 시스템 연계
- 피해: `UGameplayStatics::ApplyDamage(Target, Amount, nullptr, Caster, UDamageType::StaticClass())`

### 타게팅 구현
- Single: 마우스 히트로 액터 선택, 거리 체크(`RangeMeters * 100`)
- Circle: 지면 클릭 좌표 + 반경 오버랩(`SphereOverlapActors`), `MaxTargets` 제한
- 미리보기: `OnTargetsUpdated`/`OnTargetPointUpdated` 브로드캐스트

### UI/입력 연계
- 버튼 클릭 → `UOverlayWidgetController::RequestUseSkill(SkillID)` → `USkillExecutionSubsystem::RequestCast(Caster, Def)`로 위임
- 확정/취소 입력은 `PlayerController`에서 Subsystem 메서드 호출
- 슬롯 활성/비활성은 기존 `USkillBookComponent` 델리게이트로 유지

### 턴 시스템 연계
- `BeginNextTurn`에서 `SkillBook->OnOwnerTurnStart()` 호출
- 진행 중 캐스팅은(옵션) 턴 종료 시 `CancelCast()` 자동 호출 가드
- `CurrentRound`는 Turn Subsystem에서 관리하여 `ConfirmAndExecute` 인자로 전달

### 정책 권장 기본값
- 액션 소모: 예약 시점에 소모(취소하면 환불). 미스/세이브 성공 등 실패에도 소모는 유지.
- 쿨다운: Confirm 후에만 시작(취소/미확정은 시작하지 않음).
- 중복 캐스트: 진행 중이면 새 요청 거절.
- 캐스터 무효화: 자동 취소 + 환불 시도.

### 확장 포인트(선택)
- 실행기 패턴: `USkillDefinition`에 `TSubclassOf<USkillExecutor> ExecutorClass`
  - `USkillExecutor`가 `ComputeTargets/Resolve/Apply` 훅 제공, Subsystem은 파이프라인만 관리
- 비주얼/애니: 캐릭터 측 컴포넌트로 위임(사운드/FX/몽타주)

### 초기 구현 순서
1) `USkillExecutionSubsystem` 스캐폴딩: 상태/델리게이트/핵심 메서드
2) `RequestCast`·`CancelCast`·`ConfirmAndExecute` 골격 + `SkillBook` 연동
3) `Single` 타게팅부터 지원 → `Circle` 확장
4) UI 경로 전환: `OverlayWidgetController::RequestUseSkill`에서 Subsystem 호출
5) 온스크린 로그로 플로우 검증 → 디칼/하이라이트 연계

## SkillExecutor 개념 정리
- 역할 분리: Subsystem=흐름, Executor=스킬 고유 규칙/행동
- 권장 API 개념
  - `Initialize(Subsystem, Caster, SkillDef)`
  - 타게팅: `GetInputMode()`, `ValidateTargets()`/`SuggestTargetsFromCursor()`
  - 실행: `Resolve(OutHits)`, `Apply(ResolvedHits)`
  - 취소: `OnCanceled()`
  - 힌트: `GetTargetingShape()`(반경, 사거리)
- 구현 형태: `UObject` 파생, `Abstract/Blueprintable/EditInlineNew/DefaultToInstanced`
- 수명: “캐스트 1회 = Executor 1개”가 안전. 생성은 Subsystem이 담당. 커밋은 Subsystem만 수행.
- SkillDefinition 연계: 데이터는 Definition, 동작은 Executor. `ExecutorClass` 필드로 연결.
- 예시
  - 근접: 단일 타깃, ToHit, 명중 시 피해 롤
  - 파이어볼: 포인트 기반 원형, 세이브/절반 피해, 우호군 제외
  - 점프: 목적지 유효성/경로/낙하피해 판정 및 이동
- 실전 팁: 공통 규칙은 Subsystem, 예외는 Executor로. 애니/FX는 캐릭터 컴포넌트로 위임.

## RequestCast 최소 골격과 체크리스트
- 인터페이스
  - `RequestCast`, `CancelCast`, `SetTargets/SetTargetPoint`, `ConfirmAndExecute`, `IsBusy`, `GetState`
- 상태/멤버
  - `State`, `FCurrentCast`, 의존성 캐시, 정책/설정, 델리게이트
- 절차 요약
  - 상태 가드 → 인자 검증 → 스킬북 사용가능 → 예약 → 현재 캐스트 설정/Executor 초기화 → 상태 전환/이벤트
- 자주 빠뜨리는 부분
  - Reserve는 마지막 단계에서 호출(그 이전 실패는 환불 불필요)
  - 포인터 수명: 약참조 + `UPROPERTY`로 GC 안전성 확보
  - 재진입 가드: 동일 프레임 중복 입력 방지
  - 종료 정리: 성공/실패/취소 모든 경로에서 `State=Idle`/캐스트 초기화
  - 턴 경계: 턴 종료 시 자동 취소 옵션

---

본 문서는 싱글플레이 전제에서의 전투 실행 구조에 대한 대화 내용을 정리한 것이며, 현재 코드베이스(`USkillBookComponent`, `UBG3GameManageSubsystem`, `UOverlayWidgetController`)와 자연스럽게 결합하도록 설계되었습니다.

