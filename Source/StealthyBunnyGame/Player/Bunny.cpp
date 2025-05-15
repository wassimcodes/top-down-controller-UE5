#include "StealthyBunnyGame/Player/Bunny.h"
#include "GameFramework/CharacterMovementComponent.h"

// Input
#include "InputAction.h"

ABunny::ABunny()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup components
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));

	// Attach components
	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);


	// Setting Default Values
	bIsBunnyMoving = false;
	bIsBunnyRunning = false;
	bIsBunnyCrouching = false;

}

void ABunny::BeginPlay()
{
	Super::BeginPlay();
	GetBunnySkeletalMeshComponent();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ABunny::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABunny::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		for (const FInputActionMapping& Mapping : InputActions)
		{
			switch (Mapping.ActionType)
			{
			case EInputActionType::MOVE:
				EnhancedInputComponent->BindAction(Mapping.InputAction, ETriggerEvent::Triggered, this, &ABunny::StartMoving);
				EnhancedInputComponent->BindAction(Mapping.InputAction, ETriggerEvent::Completed, this, &ABunny::StopMoving);
				break;
			case EInputActionType::RUN:
				EnhancedInputComponent->BindAction(Mapping.InputAction, ETriggerEvent::Started, this, &ABunny::StartRunning);
				EnhancedInputComponent->BindAction(Mapping.InputAction, ETriggerEvent::Completed, this, &ABunny::StopRunning);
				break;
			case EInputActionType::CROUCH:
				EnhancedInputComponent->BindAction(Mapping.InputAction, ETriggerEvent::Started, this, &ABunny::StartCrouching);
				EnhancedInputComponent->BindAction(Mapping.InputAction, ETriggerEvent::Completed, this, &ABunny::StopCrouching);
				break;
			case EInputActionType::ORBIT:
				EnhancedInputComponent->BindAction(Mapping.InputAction, ETriggerEvent::Triggered, this, &ABunny::CameraOrbit);
				break;
			case EInputActionType::ZOOM:
				EnhancedInputComponent->BindAction(Mapping.InputAction, ETriggerEvent::Triggered, this, &ABunny::CameraZoom);
			}
		}
	}

}

void ABunny::StartMoving(const FInputActionValue& value)
{
	FVector2D Input = value.Get<FVector2D>();
	bIsBunnyMoving = true;
	
	FVector Forward = CameraComponent->GetForwardVector();
	Forward.Z = 0.0f;
	Forward.Normalize();

	FVector Right = CameraComponent->GetRightVector();
	Right.Z = 0.0f;
	Right.Normalize();

	FVector MoveDirection = (Forward * Input.Y) + (Right * Input.X);

	MoveDirection.Normalize();
	AddMovementInput(MoveDirection);
	RotateSkeletalMesh(MoveDirection);
}

void ABunny::StopMoving(const FInputActionValue& value)
{
	bIsBunnyMoving = false;

}

void ABunny::StartRunning()
{
	if (bIsBunnyMoving && !bIsBunnyCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		bIsBunnyRunning = true;
	}
}

void ABunny::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bIsBunnyRunning = false;

}

void ABunny::StartCrouching()
{
	if (!bIsBunnyRunning)
	{
		Crouch();
		bIsBunnyCrouching = true;
	}
}

void ABunny::StopCrouching()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	UnCrouch();
	bIsBunnyCrouching = false;
}

void ABunny::CameraOrbit(const FInputActionValue& value)
{   
	FVector2D Input = value.Get<FVector2D>();
	FRotator NewRotation = SpringArmComponent->GetComponentRotation();
	NewRotation.Yaw += Input.X * MouseSensitivity;
	NewRotation.Pitch -= Input.Y * MouseSensitivity;
	SpringArmComponent->SetWorldRotation(NewRotation);
}

void ABunny::CameraZoom(const FInputActionValue& value)
{
	const float MinZoom = 1000.0f;
	const float MaxZoom = 1845.0f;

	float NewOrthoWidth = CameraComponent->OrthoWidth - value.Get<float>() * MouseZoomSpeed;
	NewOrthoWidth = FMath::Clamp(NewOrthoWidth, MinZoom, MaxZoom);
	CameraComponent->OrthoWidth = NewOrthoWidth;
}



void ABunny::RotateSkeletalMesh(FVector Direction)
{
	if (Direction.IsNearlyZero())
	{
		return;
	}

	FRotator TargetRotation = Direction.Rotation();

	TargetRotation.Yaw -= 90.0f;

	FRotator CurrentRotation = GetBunnySkeletalMeshComponent()->GetComponentRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed);
	GetBunnySkeletalMeshComponent()->SetWorldRotation(NewRotation);
}


USkeletalMeshComponent* ABunny::GetBunnySkeletalMeshComponent()
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	return MeshComp;
}

