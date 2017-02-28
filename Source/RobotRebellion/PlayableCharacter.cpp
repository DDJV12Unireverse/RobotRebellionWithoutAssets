// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "PlayableCharacter.h"


#include "Kismet/HeadMountedDisplayFunctionLibrary.h"

#include "Damage.h"
#include "GlobalDamageMethod.h"
#include "GameMenu.h"
#include "WeaponBase.h"
#include "WeaponInventory.h"
#include "CustomPlayerController.h"

#include "RobotRobellionSpawnerClass.h"

#include "Assassin.h"
#include "Wizard.h"
#include "Soldier.h"
#include "Healer.h"

#include "UtilitaryMacros.h"


#define TYPE_PARSING(TypeName) "Type is "## #TypeName


APlayableCharacter::APlayableCharacter()
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

    m_spawner = CreateDefaultSubobject<URobotRobellionSpawnerClass>(TEXT("SpawnerClass"));
    m_weaponInventory = CreateDefaultSubobject<UWeaponInventory>(TEXT("WeaponInventory"));

    m_moveSpeed = 0.3f;
    m_bPressedCrouch = false;
    m_bPressedRun = false;

    //GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel2);
    GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Players");
}

void APlayableCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Set up gameplay key bindings
    check(PlayerInputComponent);
    inputOnLiving(PlayerInputComponent);
}

void APlayableCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void APlayableCharacter::TurnAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayableCharacter::LookUpAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayableCharacter::MoveForward(float Value)
{
    if((Controller != NULL) && (Value != 0.0f))
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, m_moveSpeed*Value);
    }
}

void APlayableCharacter::MoveRight(float Value)
{
    if((Controller != NULL) && (Value != 0.0f))
    {
        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get right vector 
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        // add movement in that direction
        AddMovementInput(Direction, m_moveSpeed*Value);
    }
}

///// SERVER
void APlayableCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION(APlayableCharacter, m_bPressedCrouch, COND_SkipOwner);
    DOREPLIFETIME_CONDITION(APlayableCharacter, m_bPressedRun, COND_SkipOwner);
}

// UWeaponBase* APlayableCharacter::getCurrentEquippedWeapon() const USE_NOEXCEPT
// {
//     return m_weaponInventory->getCurrentWeapon();
// }


void APlayableCharacter::ExecuteCommand(FString command) const
{
    APlayerController* MyPC = Cast<APlayerController>(Controller);
    if(MyPC)
    {
        MyPC->ConsoleCommand(command, true);
        if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, command);
        }
    }
}

///// JUMP
void APlayableCharacter::OnStartJump()
{
    if(m_bPressedCrouch)
    {
        OnCrouchToggle();
    }
    else
    {
        bPressedJump = true;
    }
}
void APlayableCharacter::OnStopJump()
{
    bPressedJump = false;
}

///// SPRINT
void APlayableCharacter::OnStartSprint()
{
    if(m_bPressedCrouch)
    {
        OnCrouchToggle();
    }
    else
    {
        //increase move speed
        m_moveSpeed = 1.0f;
        m_bPressedRun = true;

        if(Role < ROLE_Authority)
        {
            ServerSprintActivate(m_bPressedRun);
        }
    }
}

void APlayableCharacter::OnStopSprint()
{
    //decrease move speed
    m_moveSpeed = 0.3;
    m_bPressedRun = false;
    // Si nous sommes sur un client
    if(Role < ROLE_Authority)
    {
        ServerSprintActivate(m_bPressedRun);
    }
}

void APlayableCharacter::ServerSprintActivate_Implementation(bool NewRunning)
{
    if(NewRunning)
    {
        OnStartSprint();
    }
    else
    {
        OnStopSprint();
    }
}
bool APlayableCharacter::ServerSprintActivate_Validate(bool NewRunning)
{
    return true;
}

void APlayableCharacter::OnRep_SprintButtonDown()
{
    if(m_bPressedRun == true)
    {
        OnStartSprint();
    }
    else
    {
        OnStopSprint();
    }
}


/////CROUCH
void APlayableCharacter::ServerCrouchToggle_Implementation(bool NewCrouching)
{
    OnCrouchToggle();
}

bool APlayableCharacter::ServerCrouchToggle_Validate(bool NewCrouching)
{

    return true;

}

void APlayableCharacter::OnRep_CrouchButtonDown()
{

    if(m_bPressedCrouch == true)
    {
        Crouch();
    }
    else
    {
        UnCrouch();
    }
}

void APlayableCharacter::OnCrouchToggle()
{
    // Not crouched and not running -> can Crouch
    if(!IsRunning())
    {
        if(!m_bPressedCrouch)
        {
            m_bPressedCrouch = true;
            m_moveSpeed = 0.1f;
            Crouch();
        }
        else
        {
            m_bPressedCrouch = false;
            m_moveSpeed = 0.3f;
            UnCrouch();
        }
    }
    // Si nous sommes sur un client
    if(Role < ROLE_Authority)
    {
        ServerCrouchToggle(true); // le param n'a pas d'importance pour l'instant
    }
}

//////FIRE/////
void APlayableCharacter::mainFire()
{
    // Essayer de tirer un projectile
    if(Role < ROLE_Authority)
    {
        serverMainFire(); // le param n'a pas d'importance pour l'instant
    }
    else
    {
        m_weaponInventory->getCurrentWeapon()->cppAttack(this);
    }
}

void APlayableCharacter::serverMainFire_Implementation()
{
    mainFire();
}

bool APlayableCharacter::serverMainFire_Validate()
{
    return true;
}

EClassType APlayableCharacter::getClassType() const USE_NOEXCEPT
{
    return EClassType::NONE;
}

EClassType APlayableCharacter::getType() const USE_NOEXCEPT
{
    return this->getClassType();
}

/////////UI
void APlayableCharacter::openLobbyWidget()
{
    APlayerController* MyPC = Cast<APlayerController>(Controller);

    if(MyPC)
    {
        auto myHud = Cast<AGameMenu>(MyPC->GetHUD());
        myHud->DisplayWidget(myHud->LobbyImpl);
        FInputModeGameAndUI Mode;
        Mode.SetLockMouseToViewport(true);
        Mode.SetHideCursorDuringCapture(false);
        MyPC->bShowMouseCursor = true;
        MyPC->SetInputMode(Mode);
    }

    PRINT_MESSAGE_ON_SCREEN(FColor::Yellow, TEXT("Creation widget | PRESSED"));
}

///////// SWITCH WEAPON
void APlayableCharacter::switchWeapon()
{
    if(Role < ROLE_Authority)
    {
        serverSwitchWeapon(); // le param n'a pas d'importance pour l'instant
    }
    else
    {
        FString message = m_weaponInventory->toFString() + TEXT(" Go to : ");

        m_weaponInventory->switchWeapon();

        message += m_weaponInventory->toFString();

        PRINT_MESSAGE_ON_SCREEN(FColor::Yellow, message);
    }
}

void APlayableCharacter::serverSwitchWeapon_Implementation()
{
    this->switchWeapon();
}

bool APlayableCharacter::serverSwitchWeapon_Validate()
{
    return true;
}


/************************************************************************/
/* DEBUG / CHEAT                                                        */
/************************************************************************/


FString APlayableCharacter::typeToString() const USE_NOEXCEPT
{
    static const FString typeLookUpTable[EClassType::TYPE_COUNT] = {
        TYPE_PARSING(None),
        TYPE_PARSING(Soldier),
        TYPE_PARSING(Assassin),
        TYPE_PARSING(Healer),
        TYPE_PARSING(Wizard)
    };

    return typeLookUpTable[static_cast<uint8>(getClassType())];
}

void APlayableCharacter::changeInstanceTo(EClassType toType)
{
    m_spawner->spawnAndReplace(this, toType);
}

void APlayableCharacter::changeToAssassin()
{
    changeInstanceTo(EClassType::ASSASSIN);
}

void APlayableCharacter::changeToHealer()
{
    changeInstanceTo(EClassType::HEALER);
}

void APlayableCharacter::changeToSoldier()
{
    changeInstanceTo(EClassType::SOLDIER);
}

void APlayableCharacter::changeToWizard()
{
    changeInstanceTo(EClassType::WIZARD);
}

void APlayableCharacter::inputOnLiving(class UInputComponent* PlayerInputComponent)
{
    if (PlayerInputComponent)
    {
        PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayableCharacter::OnStartJump);
        PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayableCharacter::OnStopJump);

        PlayerInputComponent->BindAxis("MoveForward", this, &APlayableCharacter::MoveForward);
        PlayerInputComponent->BindAxis("MoveRight", this, &APlayableCharacter::MoveRight);

        // We have 2 versions of the rotation bindings to handle different kinds of devices differently
        // "turn" handles devices that provide an absolute delta, such as a mouse.
        // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
        PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
        PlayerInputComponent->BindAxis("TurnRate", this, &APlayableCharacter::TurnAtRate);
        PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
        PlayerInputComponent->BindAxis("LookUpRate", this, &APlayableCharacter::LookUpAtRate);

        //Crouch & Sprint
        PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayableCharacter::OnCrouchToggle);
        PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayableCharacter::OnStartSprint);
        PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayableCharacter::OnStopSprint);

        //FIRE
        PlayerInputComponent->BindAction("MainFire", IE_Pressed, this, &APlayableCharacter::mainFire);

        //ESCAPE
        PlayerInputComponent->BindAction("Escape", IE_Pressed, this, &APlayableCharacter::openLobbyWidget);
        //SWITCH WEAPON
        PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &APlayableCharacter::switchWeapon);

        /************************************************************************/
        /* DEBUG                                                                */
        /************************************************************************/
        inputDebug(PlayerInputComponent);
    }
}

void APlayableCharacter::inputOnDying(class UInputComponent* PlayerInputComponent)
{
    if (PlayerInputComponent)
    {
        //ESCAPE
        PlayerInputComponent->BindAction("Escape", IE_Pressed, this, &APlayableCharacter::openLobbyWidget);


        /************************************************************************/
        /* DEBUG                                                                */
        /************************************************************************/
        inputDebug(PlayerInputComponent);
    }
}

void APlayableCharacter::inputDebug(class UInputComponent* PlayerInputComponent)
{
    //Class change
    PlayerInputComponent->BindAction("Debug_ChangeToAssassin", IE_Pressed, this, &APlayableCharacter::changeToAssassin);
    PlayerInputComponent->BindAction("Debug_ChangeToHealer", IE_Pressed, this, &APlayableCharacter::changeToHealer);
    PlayerInputComponent->BindAction("Debug_ChangeToSoldier", IE_Pressed, this, &APlayableCharacter::changeToSoldier);
    PlayerInputComponent->BindAction("Debug_ChangeToWizard", IE_Pressed, this, &APlayableCharacter::changeToWizard);
}

void APlayableCharacter::cppOnRevive()
{
    APlayerController* playerController = Cast<APlayerController>(GetController());

    if (playerController && playerController->InputComponent)
    {
        UInputComponent* newPlayerController = CreatePlayerInputComponent();

        inputOnLiving(newPlayerController);

        playerController->InputComponent = newPlayerController;
    }

    //TODO - Continue the Revive method
}

void APlayableCharacter::cppOnDeath()
{
    FVector currentPosition = this->GetTransform().GetLocation();
    currentPosition.Z = 135.f;

    this->SetActorRotation(FRotator{ 90.0f, 0.0f, 0.0f });
    this->SetActorLocation(currentPosition);

    APlayerController* playerController = Cast<APlayerController>(GetController());

    if (playerController && playerController->InputComponent)
    {
        UInputComponent* newPlayerController = CreatePlayerInputComponent();

        inputOnDying(newPlayerController);

        playerController->InputComponent = newPlayerController;
    }
}