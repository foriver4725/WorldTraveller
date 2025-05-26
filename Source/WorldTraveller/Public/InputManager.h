#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InputManager.generated.h"

class UInputAction;

// 汎用入力を管理.
// 入力はCharacterでもらって、このスクリプトで面倒なフラグ管理を行う.
UCLASS()
class WORLDTRAVELLER_API AInputManager final : public AActor
{
	GENERATED_BODY()

public:
	AInputManager();

	static inline AInputManager* Instance() { return instance.Get(); }

	// Character から呼び出すための関数群.
	inline void InvokeInteractPressed() { bInteractPressed |= true; }
	inline void InvokeCancelPressed() { bCancelPressed |= true; }
	inline void InvokeEscapePressed() { bEscapePressed |= true; bEscapeHeld |= true; }
	inline void InvokeEscapeReleased() { bEscapeReleased |= true; bEscapeHeld &= false; }

	// フラグを公開.
	inline bool GetInteractPressed() const { return bInteractPressed; }
	inline bool GetCancelPressed() const { return bCancelPressed; }
	inline bool GetEscapePressed() const { return bEscapePressed; }
	inline bool GetEscapeHeld() const { return bEscapeHeld; }
	inline bool GetEscapeReleased() const { return bEscapeReleased; }

protected:
	virtual inline void BeginPlay() override final;
	virtual inline void Tick(float DeltaTime) override final;

private:
	// このフレームで、押下した状態 "になった" かどうか.
	bool bInteractPressed = false;
	bool bCancelPressed = false;
	bool bEscapePressed = false;

	// このフレームで、押下した状態 "である" かどうか.
	bool bEscapeHeld = false;

	// このフレームで、押下した状態 "でなくなった" かどうか.
	bool bEscapeReleased = false;

	static inline TWeakObjectPtr<AInputManager> instance = nullptr;
};
