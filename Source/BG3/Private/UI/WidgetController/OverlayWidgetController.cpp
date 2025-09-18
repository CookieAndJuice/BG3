
#include "UI/WidgetController/OverlayWidgetController.h"
#include "BG3/BG3.h"
#include "Character/BaseCharacter.h"
#include "Component/SkillBookComponent.h"
#include "Component/CharacterStatsComponent.h"
#include "Components/Overlay.h"
#include "Controller/BG3GameModePlayerController.h"
#include "Data/SkillDefinition.h"
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
        // Stats->OnHealthChanged.AddDynamic(this, &UOverlayWidgetController::HandleHealthChanged);
        // Stats->OnManaChanged.AddDynamic(this, &UOverlayWidgetController::HandleManaChanged);

        // Initial push for UI
        HandleHealthChanged(Stats->GetHealth(), Stats->GetMaxHealth());
        HandleManaChanged(Stats->GetMana(), Stats->GetMaxMana());
        OnStatsInitialized.Broadcast();
        Stats->OnFadeOut.AddUObject(this, &UOverlayWidgetController::HandleFadeOut);
    }

    if (PC && !PC->CurrentCharacterChanged.IsBound())
    {
        PC->CurrentCharacterChanged.BindUObject(this, &UOverlayWidgetController::Initialize, PC);
        PRINTLOG(TEXT("[UI] Bound PC CurrentCharacterChanged"));
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
    // 오버레이의 모든 위젯들 Fade Out
    OnFadeOutAnimationStart.Broadcast(result);

    PRINTDELEGATELOG(TEXT("FadeOut 2"));
    // 승리 or 패배 위젯 띄우기
    
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

        PRINTLOG(TEXT("[UI] Slot %s usable=%d cd=%d"), *Def->Meta.DisplayName.ToString(), (int32)bUsable, V.CooldownRemain);

        Views.Add(V);
    }

    OnActionSlotsUpdated.Broadcast(Views);
}
