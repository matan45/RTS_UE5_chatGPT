#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "RTSPlayerController.generated.h"

UCLASS()
class RTS_API ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARTSPlayerController();

	void StartPreviewBuildingSelected(class ABuilding* UIBuilding);

	//TODO when selecting building to construct show preview mesh
	// add building validation location on the terrain 
	//add UInputAction for select units and move them to location

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

	void MoveCameraForward(const FInputActionValue& Value);
	void MoveCameraRight(const FInputActionValue& Value);
	void ZoomCamera(const FInputActionValue& Value);

private:
	class ABuilding* Building;

	FVector2D CameraMovementInput;
	FVector2D CameraMovementDirection;
	float CameraZoomInput;

	class ARTSHUD* RTShud;

	UPROPERTY(EditAnywhere)
	float CameraSpeed = 2000.0f;
	UPROPERTY(EditAnywhere)
	float CameraZoomSpeed = 500.0f;
	UPROPERTY(EditAnywhere)
	float MinZoom = 300.0f;
	UPROPERTY(EditAnywhere)
	float MaxZoom = 3000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_MoveForward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_MoveRight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Zoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CameraCollisionBox;


	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);
	UFUNCTION()
	void UpdateSpringArmComponentLoction(float dt);
	UFUNCTION()
	void UpdateMiniMapPlayerIcon();
	UFUNCTION()
	FVector2D ConvertWorldToMiniMapCoordinates(FVector WorldLocation);

	void UpdateBuildingPreview();

	bool GetMouseHitLocation(FVector& OutHitLocation);
};
