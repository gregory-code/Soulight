#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SAbilityBase.generated.h"

class USAbilityDataBase;

UCLASS(BlueprintType)
class ASAbilityBase : public AActor
{
    GENERATED_BODY()

public:
    ASAbilityBase();

    UFUNCTION(BlueprintCallable, Category = "Ability")
    USAbilityDataBase* GetAbilityData() const { return AbilityData; }

    UFUNCTION(BlueprintCallable, Category = "Ability")
    FString GetAbilityName() const;

    UFUNCTION(BlueprintCallable, Category = "Ability")
    void SetAbilityOwner(ACharacter* OwningCharacter);

    UFUNCTION(BlueprintCallable, Category = "Ability")
    virtual void ExecuteAbility();

protected:
    ACharacter* OwnerCharacter;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Ability")
    USAbilityDataBase* AbilityData;

    void PlayMontage(UAnimMontage* MontageToPlay, const float& PlayRate);
};