
#include "Data/SkillDefinition.h"
#include "Animation/AnimMontage.h"
#include "Animation/Skeleton.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"

UAnimMontage* USkillDefinition::GetMontageForSkeleton(const USkeleton* Skeleton) const
{
    if (!Skeleton)
    {
        return SkillAssetSet.DefaultMontage;
    }

    for (const FSkillAnimEntry& Entry : SkillAssetSet.AnimPerSkeleton)
    {
        if (Entry.Skeleton.IsNull())
        {
            continue;
        }

        const USkeleton* EntrySkeleton = Entry.Skeleton.Get();
        if (!EntrySkeleton)
        {
            EntrySkeleton = Entry.Skeleton.LoadSynchronous();
        }

        if (EntrySkeleton != Skeleton)
        {
            continue;
        }

        UAnimMontage* Montage = Entry.Montage.Get();
        if (!Montage && !Entry.Montage.IsNull())
        {
            Montage = Entry.Montage.LoadSynchronous();
        }

        if (Montage)
        {
            return Montage;
        }
    }

    return SkillAssetSet.DefaultMontage;
}

UAnimMontage* USkillDefinition::GetMontageForMesh(const USkeletalMeshComponent* Mesh) const
{
    if (!Mesh)
    {
        return SkillAssetSet.DefaultMontage;
    }

    const USkeleton* Skeleton = nullptr;
    if (const USkeletalMesh* SkeletalMesh = Mesh->GetSkeletalMeshAsset())
    {
        Skeleton = SkeletalMesh ? SkeletalMesh->GetSkeleton() : nullptr;
    }
    

    return GetMontageForSkeleton(Skeleton);
}
