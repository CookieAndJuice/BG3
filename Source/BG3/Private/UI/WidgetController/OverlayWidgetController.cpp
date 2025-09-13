
#include "UI/WidgetController/OverlayWidgetController.h"
#include "BG3/BG3.h"
#include "Character/BaseCharacter.h"
#include "Component/SkillBookComponent.h"
#include "Component/CharacterStatsComponent.h"
#include "Data/SkillDefinition.h"
#include "UI/Widget/ActionSlotEntry.h"
#include "Game/SkillExecutionSubsystem.h"

void UOverlayWidgetController::Initialize(ABaseCharacter* InCharacter)
{
    OwningCharacter = InCharacter;
    SkillBook = InCharacter ? InCharacter->SkillBook : nullptr;
    Stats = InCharacter ? InCharacter->FindComponentByClass<UCharacterStatsComponent>() : nullptr;

    if (SkillBook)
    {
        SkillBook->OnCooldownChanged.AddDynamic(this, &UOverlayWidgetController::HandleCooldownChanged);
        SkillBook->OnUsabilityChanged.AddDynamic(this, &UOverlayWidgetController::HandleUsabilityChanged);
    }

    if (Stats)
    {
        Stats->OnHealthChanged.AddDynamic(this, &UOverlayWidgetController::HandleHealthChanged);
        Stats->OnManaChanged.AddDynamic(this, &UOverlayWidgetController::HandleManaChanged);

        // Initial push for UI
        HandleHealthChanged(Stats->GetHealth(), Stats->GetMaxHealth());
        HandleManaChanged(Stats->GetMana(), Stats->GetMaxMana());
        OnStatsInitialized.Broadcast();
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

        Views.Add(V);
    }

    OnActionSlotsUpdated.Broadcast(Views);
}
