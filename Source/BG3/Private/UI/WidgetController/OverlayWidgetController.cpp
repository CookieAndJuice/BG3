
#include "UI/WidgetController/OverlayWidgetController.h"
#include "BG3/BG3.h"
#include "Character/BaseCharacter.h"
#include "Component/SkillBookComponent.h"
#include "Data/SkillDefinition.h"
#include "UI/Widget/ActionSlotEntry.h"
#include "Game/SkillExecutionSubsystem.h"

void UOverlayWidgetController::Initialize(ABaseCharacter* InCharacter)
{
    OwningCharacter = InCharacter;
    SkillBook = InCharacter ? InCharacter->SkillBook : nullptr;

    if (SkillBook)
    {
        SkillBook->OnCooldownChanged.AddDynamic(this, &UOverlayWidgetController::HandleCooldownChanged);
        SkillBook->OnUsabilityChanged.AddDynamic(this, &UOverlayWidgetController::HandleUsabilityChanged);
    }

    RefreshSlots();
}

void UOverlayWidgetController::RefreshSlots()
{
    BuildAndBroadcast();
}

void UOverlayWidgetController::RequestUseSkill(int32 SkillID)
{
    PRINTLOG(TEXT("RequestUseSkill Called"));
    if (!SkillBook)
    {
        PRINTLOG(TEXT("SkillBook Is Not Valid"));
        return;
    }
    for (USkillDefinition* Def : SkillBook->Skills)
    {
        PRINTLOG(TEXT("%s"), *Def->GetName());
        if (Def && Def->Meta.ID == SkillID)
        {
            PRINTLOG(TEXT("Skill Found"));
            if (UWorld* World = GetWorld())
            {
                PRINTLOG(TEXT("World Found"));
                if (USkillExecutionSubsystem* SES = World->GetSubsystem<USkillExecutionSubsystem>())
                {
                    PRINTLOG(TEXT("SES Found"));
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
            else
            {
                PRINTLOG(TEXT("World is Not Valid"));
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
