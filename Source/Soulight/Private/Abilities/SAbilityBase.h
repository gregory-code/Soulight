#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SAbilityBase.generated.h"

class USAbilityDataBase;

UCLASS(BlueprintType)
class USAbilityBase : public UObject
{
    GENERATED_BODY()

public:
    USAbilityBase();

    void CopyAbility(USAbilityBase* AbilityToCopy);

    UFUNCTION(BlueprintCallable, Category = "Ability")
        USAbilityDataBase* GetAbilityData() const { return AbilityData; }

    UFUNCTION(BlueprintCallable, Category = "Ability")
        FString GetAbilityName() const;

    UFUNCTION(BlueprintCallable, Category = "Ability")
        void SetOwner(ACharacter* OwningCharacter);

    UFUNCTION(BlueprintCallable, Category = "Ability")
        virtual void ExecuteAbility();

protected:
    ACharacter* OwnerCharacter;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Ability")
    USAbilityDataBase* AbilityData;

    void PlayMontage(UAnimMontage* MontageToPlay, const float& PlayRate);
};