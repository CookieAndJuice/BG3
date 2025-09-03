
#pragma once

#include "CoreMinimal.h"

// 로그 채널
DECLARE_LOG_CATEGORY_EXTERN(BGLog, Log, All);

// 어디서 호출되는지 볼 수 있는 함수
#define CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

// CALLINFO 매크로 편하게 로그찍는 매크로
#define PRINTINFO() UE_LOG(BGLog, Warning, TEXT("%s"), *CALLINFO)

#define PRINTLOG(fmt, ...) \
UE_LOG(BGLog, Warning, TEXT("%s : %s"), *CALLINFO, *FString::Printf(fmt, ##__VA_ARGS__))