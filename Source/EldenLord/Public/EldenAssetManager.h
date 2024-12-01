// luce copyright

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "EldenAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class ELDENLORD_API UEldenAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UEldenAssetManager& Get();
protected:
	virtual void StartInitialLoading() override;
};
