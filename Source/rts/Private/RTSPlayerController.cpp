#include "RTSPlayerController.h"

#include <Kismet/GameplayStatics.h>

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RTSGameMode.h"
#include "GameTimeManager.h"


ARTSPlayerController::ARTSPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	// Create the Spring Arm Component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->TargetArmLength = 1500.0f;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 3.0f;

	// Create the Camera Component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ARTSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Access the GameMode
	ARTSGameMode* GameMode = Cast<ARTSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode)
	{
		// Access the GameTimeManager via GameMode
		AGameTimeManager* GameTimeManager = GameMode->GetGameTimeManager();

		if (GameTimeManager)
		{
			// Example: Pause the game time
			GameTimeManager->PauseTime();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GameTimeManager is not available in GameMode."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RTSGameMode not found."));
	}

	// Add the mapping context for this controller
	if (APlayerController* PC = Cast<APlayerController>(GetOwner()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(RTSMappingContext, 1);
		}
	}
}

void ARTSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &ARTSPlayerController::MoveCameraForward);
		EnhancedInputComponent->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &ARTSPlayerController::MoveCameraRight);
		EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Triggered, this, &ARTSPlayerController::ZoomCamera);
	}
}

void ARTSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpringArmComponent)
	{
		FVector NewLocation = SpringArmComponent->GetComponentLocation();
		NewLocation += FVector(CameraMovementInput.Y, CameraMovementInput.X, 0) * CameraSpeed * DeltaTime;
		SpringArmComponent->SetWorldLocation(NewLocation);

		float NewArmLength = SpringArmComponent->TargetArmLength + CameraZoomInput * CameraZoomSpeed * DeltaTime;
		SpringArmComponent->TargetArmLength = FMath::Clamp(NewArmLength, MinZoom, MaxZoom);

		CameraMovementInput = FVector2D::ZeroVector;
		CameraZoomInput = 0.0f;
	}
}

void ARTSPlayerController::MoveCameraForward(const FInputActionValue& Value)
{
	CameraMovementInput.Y = Value.Get<float>();
}

void ARTSPlayerController::MoveCameraRight(const FInputActionValue& Value)
{
	CameraMovementInput.X = Value.Get<float>();
}

void ARTSPlayerController::ZoomCamera(const FInputActionValue& Value)
{
	CameraZoomInput = Value.Get<float>();
}
