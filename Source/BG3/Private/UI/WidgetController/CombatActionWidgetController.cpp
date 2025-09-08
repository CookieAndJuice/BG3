
#include "UI/WidgetController/CombatActionWidgetController.h"

#include "Character/BaseCharacter.h"
#include "Component/SkillBookComponent.h"
#include "Data/SkillDefinition.h"

void UCombatActionWidgetController::Initialize(ABaseCharacter* InCharacter)
{
    OwningCharacter = InCharacter;
    SkillBook = InCharacter ? InCharacter->FindComponentByClass<USkillBookComponent>() : nullptr;

    if (SkillBook)
    {
        SkillBook->OnCooldownChanged.AddDynamic(this, &UCombatActionWidgetController::HandleCooldownChanged);
        SkillBook->OnUsabilityChanged.AddDynamic(this, &UCombatActionWidgetController::HandleUsabilityChanged);
    }

    RefreshSlots();
}

void UCombatActionWidgetController::RefreshSlots()
{
    BuildAndBroadcast();
}

void UCombatActionWidgetController::HandleCooldownChanged(const USkillDefinition* /*Skill*/, int32 /*NewRounds*/)
{
    BuildAndBroadcast();
}

void UCombatActionWidgetController::HandleUsabilityChanged(const USkillDefinition* /*Skill*/, bool /*bUsable*/)
{
    BuildAndBroadcast();
}

void UCombatActionWidgetController::BuildAndBroadcast()
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

        switch (Def->Cost.ActionCost)
        {
        case EActionCost::Action:
            V.ActionCost = 1;
            break;
        case EActionCost::Bonus:
            V.ActionCost = 0;
            break;
        case EActionCost::Reaction:
            V.ActionCost = 0;
            break;
        default:
            V.ActionCost = 1;
            break;
        }

        bool bUsable = false;
        SkillBook->GetUsability(Def, bUsable);
        V.bUsable = bUsable;

        V.CooldownRemain = SkillBook->GetCooldownRemaining(Def);

        Views.Add(V);
    }

    OnActionSlotsUpdated.Broadcast(Views);
}
