#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "RTSPlayerController.generated.h"

UCLASS()
class RTS_API ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARTSPlayerController();

	// Handle starting the building preview
	void StartPreviewBuildingSelected(class ABuilding* UIBuilding);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

	// Camera movement and zoom
	void MoveCameraForward(const FInputActionValue& Value);
	void MoveCameraRight(const FInputActionValue& Value);
	void ZoomCamera(const FInputActionValue& Value);
	void StartCameraRotation(const FInputActionValue& Value);
	void StopCameraRotation(const FInputActionValue& Value);

	// Edge scrolling based on mouse position
	void HandleEdgeScrolling(float DeltaTime);
	void UpdatePlayerRotation();

	// Select and cancel actions
	void Select(const FInputActionValue& Value);
	void Cancel(const FInputActionValue& Value);

private:
	// Camera movement state
	FVector2D CameraMovementInput;
	bool bIsRotatingCamera;
	FVector2D PreviousMousePosition;

	float CameraZoomInput;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraYawInput = 0.1f; // Adjust this for faster/slower rotation

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	// Input mapping and actions
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_MoveForward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_MoveRight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Zoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Select;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Cancel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Right_Click;

	// Camera movement properties
	UPROPERTY(EditAnywhere)
	float CameraSpeed = 2000.0f;

	UPROPERTY(EditAnywhere)
	float CameraZoomSpeed = 500.0f;

	UPROPERTY(EditAnywhere)
	float EdgeScrollThreshold = 50.0f;

	//void UpdateCameraComponentLocation(float DeltaTime);
	void UpdatePawnLocation(float DeltaTime);

	// Manage building previews and placement
	void UpdateBuildingPreview();
	bool GetMouseHitLocation(FVector& OutHitLocation);
	bool CanPlaceBuildingAtLocation(FVector BuildingLocation, FVector BuildingExtents);
	bool IsTerrainFlat(FVector BuildingLocation, FVector BuildingExtents, float Tolerance);
	bool IsLocationFreeOfObstacles(FVector BuildingLocation, FVector BuildingExtents);
	bool GetTerrainHeightAtLocation(const FVector& InLocation, float& OutTerrainHeight);

	// Input helpers for building selection and terrain validation
	bool PerformRaycast(FHitResult& OutHitResult, const FVector& StartLocation, const FVector& EndLocation, ECollisionChannel CollisionChannel);

	// Update minimap with player position
	void UpdateMinimap();

	class ABuilding* SelectedBuilding;
	class ABuilderUnit* SelectedBuilderUnit;
	class ARTSHUD* RTShud;
};
