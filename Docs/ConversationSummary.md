# Conversation Summary

## 이동 거리 제어 논의
- 캐릭터마다 턴당 이동 가능 거리를 설정하고, 네비게이션 경로 길이를 미리 계산해 제한을 초과하면 중간 지점으로 클램프하는 방식 제안.
- `UNavigationSystemV1::FindPathToLocationSynchronously`와 `UNavigationPath::GetPathPoints()`를 사용해 실제 경로 길이를 계산하는 방법을 공유.
- 이동 명령 시 즉시 차감하지 말고, 이동이 종료되거나 새 명령으로 교체될 때 실제 이동 거리만큼 차감하도록 `FPendingTurnMove` 상태 구조를 도입.

## 이동 거리 UI · 로깅
- `UCharacterStatsComponent`에 `OnMoveDistanceChanged` 델리게이트를 추가하고, `ResetRemainingMoveDistance`/`ConsumeMoveDistance`에서 브로드캐스트하도록 설계.
- 이동 시 소비한 거리를 `GEngine->AddOnScreenDebugMessage`로 출력하는 코드 스니펫 제공.
- `UOverlayWidgetController`가 델리게이트를 구독하여 UI에 이동 잔량을 전달하고, `UTurnEndWidget`에서 Radial Progress 업데이트를 담당하도록 구조 변환.

## 실시간 갱신 구현
- `UMouseInputComponent`가 이동 중 매 프레임 위치 변화를 추적하도록 `TickComponent`를 활성화하고, 누적 이동량을 Stats 컴포넌트에 전달하여 실시간 갱신을 구현.
- 이동 명령 시작 시 Tick을 켜고, 이동 완료 또는 취소 시 Tick을 끄는 흐름으로 마무리.
- `FPendingTurnMove` 구조를 확장(시작/최근 위치, 남은 거리)하여 중간 이동 거리 계산과 잔여 거리 관리를 담당하도록 함.

## 기타 정리
- 턴 전환(`SwitchToPawn`)에서 이동 중 상태를 리셋하여 새 캐릭터가 깨끗한 상태에서 시작하도록 수정.
- `WBP_TurnEnd`(턴 종료 위젯)에서 `UpdateMoveDistance(Remaining, Max)` 이벤트를 이용해 Radial Progress를 갱신하도록 안내.
- 대화 말미에 전체 내용을 정리한 문서를 생성해 달라는 요청이 있었고, 이 파일이 그 요약본임.
