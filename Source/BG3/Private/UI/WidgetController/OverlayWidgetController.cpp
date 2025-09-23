
#include "UI/WidgetController/OverlayWidgetController.h"
#include "BG3/BG3.h"
#include "Character/BaseCharacter.h"
#include "Component/SkillBookComponent.h"
#include "Component/CharacterStatsComponent.h"
#include "Components/Overlay.h"
#include "Controller/BG3GameModePlayerController.h"
#include "Data/SkillDefinition.h"
#include "Game/BG3GameManageSubsystem.h"
#include "UI/Widget/ActionSlotEntry.h"
#include "Game/SkillExecutionSubsystem.h"

void UOverlayWidgetController::Initialize(ABaseCharacter* InCharacter, ABG3GameModePlayerController* PC)
{
    PRINTLOG(TEXT("[UI] Initialize: Char=%s"), InCharacter ? *InCharacter->GetName() : TEXT("null"));

    // Unbind previous delegates if reinitializing
    if (SkillBook)
    {
        SkillBook->OnCooldownChanged.RemoveDynamic(this, &UOverlayWidgetController::HandleCooldownChanged);
        SkillBook->OnUsabilityChanged.RemoveDynamic(this, &UOverlayWidgetController::HandleUsabilityChanged);
    }

    if (Stats)
    {
        Stats->OnMoveDistanceChanged.RemoveDynamic(this, &UOverlayWidgetController::HandleMoveDistanceChanged);
        Stats->OnFadeOut.RemoveAll(this);
    }

    OwningCharacter = InCharacter;
    SkillBook = InCharacter ? InCharacter->SkillBook : nullptr;
    Stats = InCharacter ? InCharacter->FindComponentByClass<UCharacterStatsComponent>() : nullptr;

    if (SkillBook)
    {
        SkillBook->OnCooldownChanged.AddDynamic(this, &UOverlayWidgetController::HandleCooldownChanged);
        SkillBook->OnUsabilityChanged.AddDynamic(this, &UOverlayWidgetController::HandleUsabilityChanged);
        PRINTLOG(TEXT("[UI] Bound SkillBook delegates: %s"), *OwningCharacter->GetName());
    }

    if (Stats)
    {
        Stats->OnMoveDistanceChanged.AddDynamic(this, &UOverlayWidgetController::HandleMoveDistanceChanged);
        Stats->OnFadeOut.AddUObject(this, &UOverlayWidgetController::HandleFadeOut);

        HandleHealthChanged(Stats->GetHealth(), Stats->GetMaxHealth());
        HandleManaChanged(Stats->GetMana(), Stats->GetMaxMana());
        HandleMoveDistanceChanged(Stats->GetRemainingMoveDistance(), Stats->GetMaxMoveDistanceValue());

        OnStatsInitialized.Broadcast();
    }

    if (PC && !PC->CurrentCharacterChanged.IsBound())
    {
        PC->CurrentCharacterChanged.BindUObject(this, &UOverlayWidgetController::Initialize, PC);
        PRINTLOG(TEXT("[UI] Bound PC CurrentCharacterChanged"));
    }

    if (auto* SES = GetWorld()->GetSubsystem<USkillExecutionSubsystem>())
    {
        SES->CastingStarted.AddUObject(this, &UOverlayWidgetController::HandleCastStarted);
        SES->CastingCanceled.AddUObject(this, &UOverlayWidgetController::HandleCastCanceled);
        SES->SkillResolved.AddUObject(this, &UOverlayWidgetController::HandleCastResolved);
    }
    
    RefreshSlots();
}

void UOverlayWidgetController::RefreshSlots()
{
    BuildAndBroadcast();
}

void UOverlayWidgetController::RequestUseSkill(int32 SkillID)
{
    if (!SkillBook)
    {
        return;
    }
    for (USkillDefinition* Def : SkillBook->Skills)
    {
        if (Def && Def->Meta.ID == SkillID)
        {
            if (UWorld* World = GetWorld())
            {
                if (USkillExecutionSubsystem* SES = World->GetSubsystem<USkillExecutionSubsystem>())
                {
                    if (SES->RequestCast(OwningCharacter.Get(), Def))
                    {
                        PRINTLOG(TEXT("Skill cast started (targeting)"));
                    }
                    else
                    {
                        PRINTLOG(TEXT("Skill cast request failed"));
                    }
                }
            }
        }
    }
}

void UOverlayWidgetController::HandleCooldownChanged(const USkillDefinition* /*Skill*/, int32 /*NewRounds*/)
{
    BuildAndBroadcast();
}

void UOverlayWidgetController::HandleUsabilityChanged(const USkillDefinition* /*Skill*/, bool /*bUsable*/)
{
    BuildAndBroadcast();
}

void UOverlayWidgetController::HandleFadeOut(EResultState result)
{
    OnFadeOutAnimationStart.Broadcast(result);
}

void UOverlayWidgetController::HandleCastStarted(ABaseCharacter* Character, const USkillDefinition* Skill)
{
    USkillDefinition* InSkill = const_cast<USkillDefinition*>(Skill);
    TargetingSkill = InSkill;
    BuildAndBroadcast();
}

void UOverlayWidgetController::HandleCastCanceled()
{
    TargetingSkill = nullptr;
    BuildAndBroadcast();
}

void UOverlayWidgetController::HandleCastResolved(const FSkillResult& SkillResult)
{
    TargetingSkill = nullptr;
    BuildAndBroadcast();
}

void UOverlayWidgetController::HandleMoveDistanceChanged(float Remaining, float Max)
{
    OnMoveDistanceChanged.Broadcast(Remaining, Max);
}

void UOverlayWidgetController::HandleHealthChanged(float NewHealth, float MaxHealth)
{
    OnHealthChanged.Broadcast(NewHealth, MaxHealth);
}

void UOverlayWidgetController::HandleManaChanged(float NewMana, float MaxMana)
{
    OnManaChanged.Broadcast(NewMana, MaxMana);
}

void UOverlayWidgetController::BuildAndBroadcast()
{
    TArray<FActionSlotView> Views;

    if (!SkillBook)
    {
        OnActionSlotsUpdated.Broadcast(Views);
        return;
    }

    for (USkillDefinition* Def : SkillBook->Skills)
    {
        if (!Def) continue;

        // 스킬이 만약 이동 스킬이면, 컴뱃 패널에 추가하지 않음
        if (Def->Meta.ID == 888) return;

        FActionSlotView V;
        V.SkillID = Def->Meta.ID;
        V.DisplayName = FText::FromName(Def->Meta.DisplayName);
        V.Icon = Def->Meta.Icon.IsNull() ? nullptr : Def->Meta.Icon.LoadSynchronous();
        V.IconBG = Def->Meta.IconBG.IsNull() ? nullptr : Def->Meta.IconBG.LoadSynchronous();
        V.ActionCost = Def->Cost.ActionCost;

        bool bUsable = false;
        SkillBook->GetUsability(Def, bUsable);
        V.bUsable = bUsable;

        V.CooldownRemain = SkillBook->GetCooldownRemaining(Def);

        if (const auto* SES = GetWorld()->GetSubsystem<USkillExecutionSubsystem>())
        {
            if (TargetingSkill && TargetingSkill->Meta.ID == Def->Meta.ID)
            {
                V.bIsTargeting = SES->GetCastState() == ECastState::Targeting;
                V.bUsable = true;
            }
            
        }

        // OwningCharacter랑 GameManageSubsystem의 GetCurrentPawn이랑 같지 않으면
        // V.bUsable = false로 설정하기
        if (UBG3GameManageSubsystem* GMSub = GetWorld()->GetSubsystem<UBG3GameManageSubsystem>())
        {
            if (GMSub->GetCurrentPawn() != OwningCharacter)
            {
                V.bUsable = false;
                V.bIsTargeting = false;
            }
        }
        

        PRINTLOG(TEXT("[UI] Slot %s usable=%d cd=%d"), *Def->Meta.DisplayName.ToString(), (int32)bUsable, V.CooldownRemain);

        Views.Add(V);
    }

    OnActionSlotsUpdated.Broadcast(Views);
}



