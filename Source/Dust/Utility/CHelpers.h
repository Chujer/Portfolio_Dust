#pragma once

#include "CoreMinimal.h"
#include "UObject/ConstructorHelpers.h"

class DUST_API CHelpers
{
public:
	template<typename T>
	static T CreateComponent(AActor* InActor, TWeakObjectPtr<T>** OutComponent, FName InName)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
	}
};
