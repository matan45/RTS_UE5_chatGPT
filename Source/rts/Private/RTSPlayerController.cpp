#include "RTSPlayerController.h"

#include <Kismet/GameplayStatics.h>

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RTSGameMode.h"
#include "GameTimeManager.h"
#include "EngineUtils.h"  // Needed for TActorIterator
#include "Components/BoxComponent.h"


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

	// Initialize components
	CameraCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CameraCollisionBox"));
	CameraCollisionBox->SetupAttachment(CameraComponent); // Attach to CameraComponent
	CameraCollisionBox->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f)); // Adjust the size as needed
	CameraCollisionBox->SetCollisionProfileName(TEXT("Trigger")); // Use an appropriate collision profile
	CameraCollisionBox->SetGenerateOverlapEvents(true); // Enable overlap events
	CameraCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
}




void ARTSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Find all the triggers and bind overlap events
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor->ActorHasTag("LeftBorderTrigger") ||
			Actor->ActorHasTag("RightBorderTrigger") ||
			Actor->ActorHasTag("TopBorderTrigger") ||
			Actor->ActorHasTag("BottomBorderTrigger"))
		{
			Actor->OnActorBeginOverlap.AddDynamic(this, &ARTSPlayerController::OnOverlapBegin);
			Actor->OnActorEndOverlap.AddDynamic(this, &ARTSPlayerController::OnOverlapEnd);
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
		// Apply the combined movement input and direction
		FVector NewLocation = SpringArmComponent->GetComponentLocation();
		NewLocation += FVector(CameraMovementInput.Y + CameraMovementDirection.Y, CameraMovementInput.X + CameraMovementDirection.X, 0) * CameraSpeed * DeltaTime;
		SpringArmComponent->SetWorldLocation(NewLocation);

		// Handle camera zoom
		float NewArmLength = SpringArmComponent->TargetArmLength + CameraZoomInput * CameraZoomSpeed * DeltaTime;
		SpringArmComponent->TargetArmLength = FMath::Clamp(NewArmLength, MinZoom, MaxZoom);

		// Reset inputs after processing
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

void ARTSPlayerController::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("OnOverlapBegin"));
	if (OtherActor == GetPawn())
	{
		if (OverlappedActor->ActorHasTag("LeftBorderTrigger"))
		{
			CameraMovementDirection.X = -1.0f;
		}
		else if (OverlappedActor->ActorHasTag("RightBorderTrigger"))
		{
			CameraMovementDirection.X = 1.0f;
		}
		else if (OverlappedActor->ActorHasTag("TopBorderTrigger"))
		{
			CameraMovementDirection.Y = -1.0f;
		}
		else if (OverlappedActor->ActorHasTag("BottomBorderTrigger"))
		{
			CameraMovementDirection.Y = 1.0f;
		}
	}
}

void ARTSPlayerController::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("OnOverlapEnd"));
	if (OtherActor == GetPawn())
	{
		if (OverlappedActor->ActorHasTag("LeftBorderTrigger") || OverlappedActor->ActorHasTag("RightBorderTrigger"))
		{
			CameraMovementDirection.X = 0.0f;
		}
		else if (OverlappedActor->ActorHasTag("TopBorderTrigger") || OverlappedActor->ActorHasTag("BottomBorderTrigger"))
		{
			CameraMovementDirection.Y = 0.0f;
		}
	}
}

