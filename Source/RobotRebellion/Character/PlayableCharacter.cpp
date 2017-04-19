// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "PlayableCharacter.h"


#include "Kismet/HeadMountedDisplayFunctionLibrary.h"

#include "Gameplay/Damage/Damage.h"
#include "Global/GlobalDamageMethod.h"
#include "UI/GameMenu.h"
#include "Gameplay/Weapon/WeaponBase.h"
#include "Gameplay/Weapon/WeaponInventory.h"
#include "CustomPlayerController.h"
#include "IA/Controller/DroneAIController.h"
#include "Gameplay/Item/PickupActor.h"

#include "Gameplay/Debug/RobotRobellionSpawnerClass.h"

#include "Assassin.h"
#include "Wizard.h"
#include "Soldier.h"
#include "Healer.h"
#include "Drone.h"
#include "IA/Character/RobotsCharacter.h"

#include "Tool/UtilitaryMacros.h"
#include "Global/EntityDataSingleton.h"
#include "Global/RobotRebellionGameMode.h"
#include "Global/RobotRebellionGameState.h"


#define TYPE_PARSING(TypeName) "Type is "## #TypeName

#define STAND_UP_HEIGHT 70.f
#define CROUCH_HEIGHT -10.f


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
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Slight camera offset to aid with object selection
    //CameraBoom->SocketOffset = FVector(0, 35, 0);
    CameraBoom->TargetOffset = FVector(0.f, 0.f, STAND_UP_HEIGHT);

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

                                                   // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
                                                   // are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)


    m_spawner = CreateDefaultSubobject<URobotRobellionSpawnerClass>(TEXT("SpawnerClass"));
    m_weaponInventory = CreateDefaultSubobject<UWeaponInventory>(TEXT("WeaponInventory"));
    m_spellKit = CreateDefaultSubobject<USpellKit>(TEXT("SpellKit"));

    m_moveSpeed = 0.3f;
    m_bPressedCrouch = false;
    m_bPressedRun = false;

    MinUseDistance = 75.0f;
    MaxUseDistance = 250.0f;
    PrimaryActorTick.bCanEverTick = true;
    //GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel2);
    GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Players");

    m_fpsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPS Mesh"));
    m_fpsMesh->SetupAttachment(GetCapsuleComponent());
    m_fpsMesh->SetVisibility(false);


    //Revive

    m_isReviving = false;

    this->deactivatePhysicsKilledMethodPtr = &APlayableCharacter::doesNothing;
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

    m_manaPotionsCount = m_nbManaPotionStart;
    m_bombCount = m_nbBombStart;
    m_healthPotionsCount = m_nbHealthPotionStart;
    CameraBoom->TargetArmLength = m_TPSCameraDistance; // The camera follows at this distance behind the character	

    m_tpsMode = true;

    // InputMode UI to select classes
    APlayerController* MyPC = Cast<APlayerController>(GetController());
    if(MyPC)
    {
        this->EnablePlayInput(false);
    }

}

void APlayableCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(Controller && Controller->IsLocalController())
    {
        AActor* usable = Cast<AActor>(GetUsableInView());
        // Terminer le focus sur l'objet pr�c�dent
        if(focusedPickupActor != usable)
        {
            m_isReviving = false;
            m_currentRevivingTime = 0.f;
            //PRINT_MESSAGE_ON_SCREEN(FColor::Red, "Lost Focused");
            bHasNewFocus = true;
        }
        // Assigner le nouveau focus (peut �tre nul)
        focusedPickupActor = usable;
        // D�marrer un nouveau focus si Usable != null;
        if(usable && usable->GetName() != "Floor")
        {
            if(bHasNewFocus)
            {
                bHasNewFocus = false;
                // only debug utility
                //PRINT_MESSAGE_ON_SCREEN(FColor::Yellow, TEXT("Focus"));
            }
            if(m_isReviving)
            {
                m_currentRevivingTime += DeltaTime;

                if(m_currentRevivingTime >= m_requiredTimeToRevive)
                {
                    m_isReviving = false;
                    m_currentRevivingTime = 0.f;

                    APlayableCharacter* charac = Cast<APlayableCharacter>(focusedPickupActor);
                    if(charac)
                    {
                        cppPreRevive(charac);
                    }
                }
            }
        }
    }

    (this->*deactivatePhysicsKilledMethodPtr)();

    this->updateIfInCombat();

    //PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Red, "Combat : " + FString::FromInt(m_isInCombat));
}

void APlayableCharacter::updateIfInCombat()
{
    EntityDataSingleton& data = EntityDataSingleton::getInstance();

    TArray<ARobotsCharacter*>& robots = data.m_robotArray;
    uint32 robotCount = robots.Num();

    for(uint32 iter = 0; iter < robotCount; ++iter)
    {
        ARobotsCharacter* rob = robots[iter];
        if(rob && !rob->IsPendingKillOrUnreachable() && rob->m_isInCombat)
        {
            m_isInCombat = true;
            return;
        }
    }

    m_isInCombat = false;
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
    DOREPLIFETIME_CONDITION(APlayableCharacter, m_bombCount, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(APlayableCharacter, m_healthPotionsCount, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(APlayableCharacter, m_manaPotionsCount, COND_OwnerOnly);
}


void APlayableCharacter::ExecuteCommand(FString command) const
{
    APlayerController* MyPC = Cast<APlayerController>(Controller);
    if(MyPC)
    {
        MyPC->ConsoleCommand(command, true);
        PRINT_MESSAGE_ON_SCREEN(FColor::Red, command);
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

            CameraBoom->TargetOffset.Z = CROUCH_HEIGHT;
            this->BaseEyeHeight = CROUCH_HEIGHT;

            Crouch();
        }
        else
        {
            m_bPressedCrouch = false;
            m_moveSpeed = 0.3f;

            CameraBoom->TargetOffset.Z = STAND_UP_HEIGHT;
            this->BaseEyeHeight = STAND_UP_HEIGHT;

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

//DEAD
//Function to call in BP, can't do it with macro
bool APlayableCharacter::isDeadBP()
{
    return isDead();
}

////// SPELL CAST /////
bool APlayableCharacter::castSpellServer_Validate(int32 index)
{
    return true;
}

void APlayableCharacter::castSpellServer_Implementation(int32 index)
{
    castSpell(index);
}

void APlayableCharacter::castSpell(int32 index)
{
    m_spellKit->cast(index);
}

//TYPE
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
        if(myHud->LobbyImpl->IsVisible())
        {
            closeLobbyWidget();
            return;
        }
        myHud->DisplayWidget(myHud->LobbyImpl);
        giveInputGameMode(false);
    }
}

void APlayableCharacter::closeLobbyWidget()
{
    APlayerController* MyPC = Cast<APlayerController>(Controller);

    if(MyPC)
    {
        auto myHud = Cast<AGameMenu>(MyPC->GetHUD());
        myHud->HideWidget(myHud->LobbyImpl);
        giveInputGameMode(true);
    }
}

void APlayableCharacter::closeSelectionWidget()
{
    APlayerController* MyPC = Cast<APlayerController>(this->GetController());
    if(MyPC)
    {
        auto myHud = Cast<AGameMenu>(MyPC->GetHUD());
        myHud->HideWidget(myHud->ClassSelectionWidgetImpl);
        myHud->DisplayWidget(myHud->HUDCharacterImpl);
        giveInputGameMode(true);
    }
}

void APlayableCharacter::giveInputGameMode(bool status)
{
    ACustomPlayerController* MyPC = Cast<ACustomPlayerController>(this->GetController());
    if(MyPC)
    {
        EnablePlayInput(status);
        MyPC->setInputMode(status);
    }
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

void APlayableCharacter::interactBegin()
{
    interact(GetUsableInView());
}

void APlayableCharacter::interact(AActor* focusedActor)
{
    if(Role >= ROLE_Authority)
    {
        APickupActor* Usable = Cast<APickupActor>(focusedActor);
        APlayableCharacter* deadBody = Cast<APlayableCharacter>(focusedActor);
        ADrone* drone = Cast<ADrone>(focusedActor);
        if(Usable) //focusedActor is an Usable Object
        {
            if(Usable->getObjectType() == EObjectType::MANA_POTION)
            {
                if(m_manaPotionsCount < m_nbManaPotionMax)
                {
                    clientInteract(Usable);
                    ++m_manaPotionsCount;
                }

            }
            else if(Usable->getObjectType() == EObjectType::HEALTH_POTION)
            {
                if(m_healthPotionsCount < m_nbHealthPotionMax)
                {
                    clientInteract(Usable);
                    ++m_healthPotionsCount;
                }
            }
            else if(Usable->getObjectType() == EObjectType::BOMB)
            {
                if(m_bombCount < m_nbBombMax)
                {
                    clientInteract(Usable);
                    ++m_bombCount;
                }

            }
            else
            {
                PRINT_MESSAGE_ON_SCREEN(FColor::Blue, TEXT("INVALID OBJECT"));
            }
        }
        else if(deadBody && deadBody->isDead() && m_currentRevivingTime < m_requiredTimeToRevive) //Focused Actor is a corpse
        {
            PRINT_MESSAGE_ON_SCREEN(FColor::Blue, TEXT("Dead Body"));
            clientRevive();
            m_isReviving = true;
        }
        else if(drone)
        {
            ADroneAIController* droneController = Cast<ADroneAIController>(drone->GetController());
            if(droneController)
            {
                PRINT_MESSAGE_ON_SCREEN(FColor::Purple, "InteractDroneControler");
                giveBombToDrone(droneController);
            }
        }
    }
    else
    {
        serverInteract(focusedActor);
    }
}


void APlayableCharacter::serverInteract_Implementation(AActor* focusedActor)
{
    this->interact(focusedActor);
}

bool APlayableCharacter::serverInteract_Validate(AActor* focusedActor)
{
    return true;
}

void APlayableCharacter::interactEnd()
{
    m_isReviving = false;
    m_currentRevivingTime = 0.f;
}


void APlayableCharacter::giveBombToDrone(ADroneAIController* drone)
{
    if(Role >= ROLE_Authority)
    {
        if(!drone->HasABomb() && m_bombCount > 0)
        {
            drone->receiveBomb();
            --m_bombCount;
            PRINT_MESSAGE_ON_SCREEN(FColor::Purple, "ServergiveBombToDrone");
        }
        return;
    }
    PRINT_MESSAGE_ON_SCREEN(FColor::Purple, "giveBombToDrone");
    serverGiveBombToDrone(drone);
}

void APlayableCharacter::serverGiveBombToDrone_Implementation(ADroneAIController* drone)
{
    if(!drone->HasABomb() && m_bombCount > 0)
    {
        drone->receiveBomb();
        --m_bombCount;
        PRINT_MESSAGE_ON_SCREEN(FColor::Red, "ServergiveBombToDrone");
    }
}

bool APlayableCharacter::serverGiveBombToDrone_Validate(ADroneAIController* drone)
{
    return true;
}
void APlayableCharacter::serverSwitchWeapon_Implementation()
{
    this->switchWeapon();
}

void APlayableCharacter::clientInteract_Implementation(APickupActor* Usable)
{
    Usable->OnPickup(this);
}


void APlayableCharacter::OnPickup(APawn * InstigatorPawn)
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Yellow, "focusActor")
}

void APlayableCharacter::updateAllCharacterBillboard_Implementation(UCameraComponent* camToFollow)
{
    TArray<AActor*> OutArray;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARobotRebellionCharacter::StaticClass(), OutArray);

    for(AActor* charc : OutArray)
    {
        ARobotRebellionCharacter* RRCharac = Cast<ARobotRebellionCharacter>(charc);
        if(RRCharac)
        {
            RRCharac->setBillboardInstanceNewCamera(camToFollow);
        }
    }
}

void APlayableCharacter::clientRevive_Implementation()
{
    m_isReviving = true;
}

bool APlayableCharacter::serverSwitchWeapon_Validate()
{
    return true;
}


/************************************************************************/
/* DEBUG / CHEAT                                                        */
/************************************************************************/


void APlayableCharacter::onDebugCheat()
{
    if(Role == ROLE_Authority)
    {
        if(this->isImmortal())
        {
            this->setImmortal(false);
        }
        else
        {
            this->setImmortal(true);
        }
        PRINT_MESSAGE_ON_SCREEN_UNCHECKED(FColor::Cyan, FString::Printf(TEXT("Immortality Status : %d"), this->isImmortal()));
    }
    else
    {
        serverOnDebugCheat();
    }
}

bool APlayableCharacter::serverOnDebugCheat_Validate()
{
    return true;
}

void APlayableCharacter::serverOnDebugCheat_Implementation()
{
    onDebugCheat();
}


FString APlayableCharacter::typeToString() const USE_NOEXCEPT
{
    static const FString typeLookUpTable[] = {
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
    UWorld* w = this->GetWorld();
    ARobotRebellionGameState* gameState = Cast<ARobotRebellionGameState>(w->GetGameState());
    if(gameState)
    {
        gameState->setStartGameMode();
    }
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
    if(PlayerInputComponent)
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
        /* Removed cause feature cut
        PlayerInputComponent->BindAction("SecondFire", IE_Pressed, this, &APlayableCharacter::secondFire);
        */

        //ESCAPE
        PlayerInputComponent->BindAction("Escape", IE_Pressed, this, &APlayableCharacter::openLobbyWidget);

        //SWITCH WEAPON
        PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &APlayableCharacter::switchWeapon);

        //INTERACT
        PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayableCharacter::interactBegin);
        PlayerInputComponent->BindAction("Interact", IE_Released, this, &APlayableCharacter::interactEnd);

        //SPELLS
        PlayerInputComponent->BindAction("Spell1", IE_Pressed, this, &APlayableCharacter::castSpellInputHanlder<0>);
        PlayerInputComponent->BindAction("Spell2", IE_Pressed, this, &APlayableCharacter::castSpellInputHanlder<1>);
        PlayerInputComponent->BindAction("Spell3", IE_Pressed, this, &APlayableCharacter::castSpellInputHanlder<2>);
        /* Removed cause feature cut
        PlayerInputComponent->BindAction("Spell4", IE_Pressed, this, &APlayableCharacter::castSpellInputHanlder<3>);
        */

        //USE OBJECTS
        PlayerInputComponent->BindAction("LifePotion", IE_Pressed, this, &APlayableCharacter::useHealthPotion);
        PlayerInputComponent->BindAction("ManaPotion", IE_Pressed, this, &APlayableCharacter::useManaPotion);

        //VIEW
        /* Remove to ensure we dont switch to FPV during presentation
        PlayerInputComponent->BindAction("SwitchView", IE_Pressed, this, &APlayableCharacter::switchView);
        */

        //CHANGE MAP
        PlayerInputComponent->BindAction("Debug_GotoDesert", IE_Released, this, &APlayableCharacter::gotoDesert);
        PlayerInputComponent->BindAction("Debug_GotoRuins", IE_Released, this, &APlayableCharacter::gotoRuins);
        PlayerInputComponent->BindAction("Debug_GotoGym", IE_Released, this, &APlayableCharacter::gotoGym);

        PlayerInputComponent->BindAction("Debug_CheatCode", IE_Released, this, &APlayableCharacter::onDebugCheat);

        /************************************************************************/
        /* DEBUG                                                                */
        /************************************************************************/
        inputDebug(PlayerInputComponent);
    }
}

void APlayableCharacter::inputOnDying(class UInputComponent* PlayerInputComponent)
{
    if(PlayerInputComponent)
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

    //Display Drone UT Scores
    PlayerInputComponent->BindAction("Debug_DroneDisplay", IE_Pressed, this, &APlayableCharacter::enableDroneDisplay);
}

void APlayableCharacter::cppPreRevive(APlayableCharacter* characterToRevive)
{
    if(Role >= ROLE_Authority)
    {
        characterToRevive->restoreHealth(characterToRevive->getMaxHealth() / 2);
        PRINT_MESSAGE_ON_SCREEN(FColor::Red, "Prerevive");
        return;
    }

    this->serverCppPreRevive(characterToRevive);
}

void APlayableCharacter::serverCppPreRevive_Implementation(APlayableCharacter* characterToRevive)
{
    cppPreRevive(characterToRevive);
}

bool APlayableCharacter::serverCppPreRevive_Validate(APlayableCharacter* characterToRevive)
{
    return true;
}

void APlayableCharacter::cppOnRevive()
{
    this->EnablePlayInput(true);
    this->activatePhysics(true);

    PRINT_MESSAGE_ON_SCREEN(FColor::Blue, TEXT("Revive"));
}

void APlayableCharacter::cppOnDeath()
{
    if(!this->GetMovementComponent()->IsFalling())
    {
        this->activatePhysics(false);
    }
    else
    {
        this->deactivatePhysicsKilledMethodPtr = &APlayableCharacter::deactivatePhysicsWhenKilled;
    }

    this->EnablePlayInput(false);
    this->m_alterationController->removeAllAlteration();
    this->m_currentRevivingTime = 0.f;
}

void APlayableCharacter::deactivatePhysicsWhenKilled()
{
    if(this->GetMovementComponent()->IsFalling())
    {
        return;
    }
    this->activatePhysics(false);
    this->deactivatePhysicsKilledMethodPtr = &APlayableCharacter::doesNothing;
}


void APlayableCharacter::EnablePlayInput(bool enable)
{
    APlayerController* playerController = Cast<APlayerController>(GetController());

    if(playerController && playerController->InputComponent)
    {
        UInputComponent* newPlayerController = CreatePlayerInputComponent();

        if(enable)
        {
            inputOnLiving(newPlayerController);
        }
        else
        {
            inputOnDying(newPlayerController);
        }

        playerController->InputComponent = newPlayerController;
    }

    if(Role >= ROLE_Authority)
    {
        clientEnableInput(enable);
    }
}

GENERATE_IMPLEMENTATION_METHOD_AND_DEFAULT_VALIDATION_METHOD(APlayableCharacter, clientEnableInput, bool enableInput)
{
    APlayerController* playerController = Cast<APlayerController>(GetController());

    if(playerController && playerController->InputComponent)
    {
        UInputComponent* newPlayerController = CreatePlayerInputComponent();

        if(enableInput)
        {
            inputOnLiving(newPlayerController);
        }
        else
        {
            inputOnDying(newPlayerController);
        }

        playerController->InputComponent = newPlayerController;
    }
}

AActor* APlayableCharacter::GetUsableInView()
{
    FVector CamLoc;
    FVector eyeLoc;
    FRotator CamRot;
    FRotator eyeRot;

    if(Controller == NULL)
    {
        return NULL;
    }

    GetActorEyesViewPoint(eyeLoc, eyeRot);
    Controller->GetPlayerViewPoint(CamLoc, CamRot);

    const FVector Direction = CamRot.Vector();
    const FVector TraceStart = GetActorLocation() + Direction * MinUseDistance + FVector(0.0f, 0.0f, BaseEyeHeight);
    const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance);

    FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, this);
    TraceParams.bTraceAsyncScene = true;
    TraceParams.bReturnPhysicalMaterial = false;
    //TraceParams.bTraceComplex = true;
    FHitResult Hit(ForceInit);
    GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_WorldStatic, TraceParams);

    //TODO: Comment or remove once implemented in post-process.
    //DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f);

    return (Hit.GetActor());
}

//////INVENTORY///////
void APlayableCharacter::useHealthPotion()
{
    if(Role < ROLE_Authority)
    {
        serverUseHealthPotion();
    }
    else if(m_healthPotionsCount > 0 && getHealth() < getMaxHealth())
    {
        restoreHealth(m_healthPerPotion);
        --m_healthPotionsCount;
    }
}

void APlayableCharacter::serverUseHealthPotion_Implementation()
{
    useHealthPotion();
}

bool APlayableCharacter::serverUseHealthPotion_Validate()
{
    return true;
}

void APlayableCharacter::useManaPotion()
{
    if(Role < ROLE_Authority)
    {
        serverUseManaPotion();
    }
    else if(m_manaPotionsCount > 0 && getMana() < getMaxMana())
    {
        restoreMana(m_manaPerPotion);
        --m_manaPotionsCount;
    }
}

void APlayableCharacter::serverUseManaPotion_Implementation()
{
    useManaPotion();
}

bool APlayableCharacter::serverUseManaPotion_Validate()
{
    return true;
}

void APlayableCharacter::setManaPotionCount(int nbPotions)
{
    if(nbPotions > m_nbManaPotionMax)
    {
        m_manaPotionsCount = m_nbManaPotionMax;
    }
    else
    {
        m_manaPotionsCount = nbPotions;
    }
}

void APlayableCharacter::setHealthPotionCount(int nbPotions)
{
    if(nbPotions > m_nbHealthPotionMax)
    {
        m_healthPotionsCount = m_nbHealthPotionMax;
    }
    else
    {
        m_healthPotionsCount = nbPotions;
    }
}

void APlayableCharacter::setBombCount(int nbBombs)
{
    if(nbBombs > m_nbBombMax)
    {
        m_bombCount = m_nbBombMax;
    }
    else
    {
        m_bombCount = nbBombs;
    }
}

void APlayableCharacter::loseBomb()
{
    m_bombCount = 0;
    PRINT_MESSAGE_ON_SCREEN(FColor::Turquoise, TEXT("BombLost"));

    if(Role < ROLE_Authority)
    {
        serverLoseBomb();
    }
}

void APlayableCharacter::serverLoseBomb_Implementation()
{
    loseBomb();
}

bool APlayableCharacter::serverLoseBomb_Validate()
{
    return true;
}

void APlayableCharacter::gotoDesert()
{
    if(Role == ROLE_Authority)
    {
        GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/Desert", true, true);
    }
    else
    {
        serverGotoDesert();
    }
}

void APlayableCharacter::gotoRuins()
{
    if(Role == ROLE_Authority)
    {
        GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/Ruins", true, true);
    }
    else
    {
        serverGotoRuins();
    }
}

void APlayableCharacter::gotoGym()
{
    if(Role == ROLE_Authority)
    {
        GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap", true, true);
    }
    else
    {
        serverGotoGym();
    }
}
void APlayableCharacter::serverGotoDesert_Implementation()
{
    gotoDesert();
}

bool APlayableCharacter::serverGotoDesert_Validate()
{
    return true;
}

void APlayableCharacter::serverGotoGym_Implementation()
{
    gotoGym();
}

bool APlayableCharacter::serverGotoGym_Validate()
{
    return true;
}

void APlayableCharacter::serverGotoRuins_Implementation()
{
    gotoRuins();
}

bool APlayableCharacter::serverGotoRuins_Validate()
{
    return true;
}

void APlayableCharacter::switchView()
{
    if(m_tpsMode)
    {
        CameraBoom->TargetArmLength = m_FPSCameraDistance;
    }
    else
    {
        CameraBoom->TargetArmLength = m_TPSCameraDistance;
    }

    m_tpsMode = !m_tpsMode;

    UMeshComponent* characterMesh = FindComponentByClass<UMeshComponent>();
    if(m_isInvisible)
    {
        if(characterMesh)
        {
            characterMesh->SetVisibility(false);
            m_fpsMesh->SetVisibility(false);
        }
    }
    else
    {
        if(characterMesh)
        {
            characterMesh->SetVisibility(m_tpsMode);
            m_fpsMesh->SetVisibility(!m_tpsMode);
        }
    }
}

UMeshComponent * APlayableCharacter::getCurrentViewMesh()
{
    UMeshComponent* characterMesh = FindComponentByClass<UMeshComponent>();
    return m_tpsMode ? characterMesh : m_fpsMesh;
}

void APlayableCharacter::activatePhysics(bool mustActive)
{
    if(mustActive)
    {
        //this->GetCapsuleComponent()->CreatePhysicsState();
        GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Players");
    }
    else
    {
        //this->GetCapsuleComponent()->DestroyPhysicsState();
        GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Dead");
    }

    if(Role >= ROLE_Authority)
    {
        multiActivatePhysics(mustActive);
    }
}

bool APlayableCharacter::multiActivatePhysics_Validate(bool mustActive)
{
    return true;
}

void APlayableCharacter::multiActivatePhysics_Implementation(bool mustActive)
{
    if(mustActive)
    {
        //this->GetCapsuleComponent()->CreatePhysicsState();
        GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Players");
    }
    else
    {
        //this->GetCapsuleComponent()->DestroyPhysicsState();
        GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("Dead");

    }
}


void APlayableCharacter::enableDroneDisplay()
{
    ADroneAIController* droneController = nullptr;
    TArray<AActor*> drone;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADroneAIController::StaticClass(), drone);
    if(drone.Num() > 0) //The king is here
    {
        droneController = Cast<ADroneAIController>(drone.Top());

    }
    if(droneController)
    {
        droneController->enableDroneDisplay(!droneController->isDebugEnabled());
    }
}