// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RobotRebellion.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "RobotRebellionCharacter.h"

#include "Damage.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"



//////////////////////////////////////////////////////////////////////////
// ARobotRebellionCharacter

ARobotRebellionCharacter::ARobotRebellionCharacter() : Attributes(1000, 1000, 100, 100, 10, 5, 6)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

}

//////////////////////////////////////////////////////////////////////////
// Input

void ARobotRebellionCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARobotRebellionCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARobotRebellionCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARobotRebellionCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARobotRebellionCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARobotRebellionCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARobotRebellionCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ARobotRebellionCharacter::OnResetVR);
}


void ARobotRebellionCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ARobotRebellionCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ARobotRebellionCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ARobotRebellionCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARobotRebellionCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARobotRebellionCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);


        // A retirer

        int b = 2;

        Damage test(this, this);

        unsigned int k = test([b](const ARobotRebellionCharacter* assaillant, const ARobotRebellionCharacter* receiver) {
                float intermediary = static_cast<float>(assaillant->getStrength()) - static_cast<float>(receiver->getDefense());
                if (intermediary < 1.f)
                {
                    intermediary = 1.f;
                }

                return b * static_cast<Damage::DamageValue>(static_cast<float>(assaillant->getAgility()) / static_cast<float>(receiver->getAgility()) * intermediary);
            },
            3.f
        );

        this->inflictDamage(k);

        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Dammage %d"));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, 
                                         TEXT(
                                             "received Damage : " + 
                                             FString::FromInt(k) + 
                                             " Current PV : " + 
                                             FString::FromInt(this->getHealth()) + 
                                             "/" + 
                                             FString::FromInt(this->getMaxHealth())
                                         )
        );
	}
}

void ARobotRebellionCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
