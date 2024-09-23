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

	// handle player movement and zoom
	void MovePlayerForwardAndBackward(const FInputActionValue& Value);
	void MovePlayerRightAndLeft(const FInputActionValue& Value);
	void ZoomInAndOut(const FInputActionValue& Value);
	void StartPlayerRotation(const FInputActionValue& Value);
	void StopPlayerRotation(const FInputActionValue& Value);
	void UpdatePlayerRotation();

	// Edge scrolling based on mouse position
	void HandleEdgeScrolling(float DeltaTime);

	// Select and cancel actions
	void Select(const FInputActionValue& Value);
	void Cancel(const FInputActionValue& Value);

private:
	// Player movement state
	FVector2D PlayerMouseLocation;
	bool IsPlayerRotating;
	FVector2D PreviousPlayerMouseLocation;

	float PlayerZoomInput;
	float PlayerYawInput = 0.1f;

	UPROPERTY(EditAnywhere)
	float PlayerSpeed = 2000.0f;

	UPROPERTY(EditAnywhere)
	float PlayerZoomSpeed = 500.0f;

	// Edge scrolling
	UPROPERTY(EditAnywhere)
	float EdgeScrollThreshold = 50.0f;

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
	//TODO add middle mouse action for rotate preview building

	//void UpdatePlayerLocation(float DeltaTime);
	void UpdatePlayerLocation(float DeltaTime);

	// Manage building previews and placement
	void UpdateBuildingPreview();


	// helper functions
	bool GetMouseHitLocation(FVector& OutHitLocation);
	bool CanPlaceBuildingAtLocation(FVector BuildingLocation, FVector BuildingExtents);
	bool IsTerrainFlat(FVector BuildingLocation, FVector BuildingExtents, float Tolerance);
	bool IsLocationFreeOfObstacles(FVector BuildingLocation, FVector BuildingExtents);
	bool GetTerrainHeightAtLocation(const FVector& InLocation, float& OutTerrainHeight);
	bool PerformRaycast(FHitResult& OutHitResult, const FVector& StartLocation, const FVector& EndLocation, ECollisionChannel CollisionChannel);
	void Deselect();

	// Update minimap with player position
	void UpdateMinimap();

	class ABuilding* SelectedBuilding;
	class ABuilderUnit* SelectedBuilderUnit;
	class ARTSHUD* RTShud;
};
