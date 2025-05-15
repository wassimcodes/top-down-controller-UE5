
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// Access Components
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"


// Access Input
#include "EnhancedInputComponent.h"



#include "Bunny.generated.h"

UENUM()
enum class EInputActionType : uint8
{
	MOVE,
	RUN,
	CROUCH,
	COVER,
	ORBIT,
	ZOOM,
};

USTRUCT()
struct FInputActionMapping
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	EInputActionType ActionType;

	UPROPERTY(EditAnywhere)
	UInputAction* InputAction;
};


UCLASS()
class STEALTHYBUNNYGAME_API ABunny : public ACharacter
{
	GENERATED_BODY()

public:
	ABunny();

protected:
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere,Category = "Components")
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, Category = "Input")
	TArray<FInputActionMapping> InputActions;

	


public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	// Action Methods
	void StartMoving(const FInputActionValue& value);
	void StopMoving(const FInputActionValue& value);
	void StartRunning();
	void StopRunning();
	void StartCrouching();
	void StopCrouching();
	void CameraOrbit(const FInputActionValue& value);
	void CameraZoom(const FInputActionValue& value);

public:
	// Movement Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed;						
	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Movement")
	float RunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MouseSensitivity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MouseZoomSpeed;


private:
	void RotateSkeletalMesh(FVector Direction);
	USkeletalMeshComponent* GetBunnySkeletalMeshComponent();
	
private:
	bool bIsBunnyMoving;
	bool bIsBunnyRunning;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsBunnyCrouching;
};
