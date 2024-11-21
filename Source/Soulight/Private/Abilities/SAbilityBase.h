#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SAbilityBase.generated.h"

class USAbilityDataBase;


/*
*   TODO: Add Ability Cooldowns;
*/
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

    UFUNCTION(BlueprintCallable, Category = "Ability")
    virtual void CancelAbility();

    UFUNCTION(BlueprintCallable, Category = "Ability")
    virtual void EndAbility();

    UFUNCTION()
    bool GetAbilityActive() { return bIsAbilityActive; }

protected:
    ACharacter* OwnerCharacter;

    UPROPERTY()
    bool bIsAbilityActive = false;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Ability")
    USAbilityDataBase* AbilityData;

protected:
    void PlayMontage(UAnimMontage* MontageToPlay, const float& PlayRate);
    void StopMontage(UAnimMontage* MontageToStop);

};