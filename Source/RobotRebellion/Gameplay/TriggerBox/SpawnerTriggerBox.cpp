// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "SpawnerTriggerBox.h"

#include "Character/King.h"
#include "Character/NonPlayableCharacter.h"
#include "IA/Controller/CustomAIControllerBase.h"
#include "Global/EntityDataSingleton.h"
#include "Character/PlayableCharacter.h"


ASpawnerTriggerBox::ASpawnerTriggerBox()
{
    UShapeComponent* collisionComponent = GetCollisionComponent();
    collisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    collisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
    collisionComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
    collisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
    collisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECollisionResponse::ECR_Overlap);
    collisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel7, ECollisionResponse::ECR_Overlap);
    collisionComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASpawnerTriggerBox::onHit);

    m_spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
}

void ASpawnerTriggerBox::BeginPlay()
{
    Super::BeginPlay();

    check(m_populationTransform.Num() == m_populationToSpawn.Num());
}

void ASpawnerTriggerBox::onHit(UPrimitiveComponent* var1, AActor* enteredActor, UPrimitiveComponent* var3, int32 var4, bool var5, const FHitResult& var6)
{
    if(m_activeOnlyWhenKingHere && !Cast<AKing>(enteredActor))
    {
        return;
    }

    if(m_noSpawnWhenPopulated && m_spawned.Num() != 0)
    {
        return;
    }

    if(Role >= ROLE_Authority && m_autoActivateCombat)
    {
        this->checkCharactersOnBox();
    }

    this->spawnEnnemies();

    if(m_destroyWhenPassed)
    {
        this->killItself();
    }
}

void ASpawnerTriggerBox::checkCharactersOnBox()
{
    FVector volumeCenter;
    FVector extent;
    this->GetActorBounds(false, volumeCenter, extent);

    m_maxDist = volumeCenter.X + (extent.X + m_scope + 1000.f);

    EntityDataSingleton& datas = EntityDataSingleton::getInstance();

    auto containPredicate = [
        minX = volumeCenter.X - (extent.X + m_scope),
            maxX = volumeCenter.X + (extent.X + m_scope),
            minY = volumeCenter.Y - (extent.Y + m_scope),
            maxY = volumeCenter.Y + (extent.Y + m_scope),
            minZ = volumeCenter.Z - (extent.Z + m_scope),
            maxZ = volumeCenter.Z + (extent.Z + m_scope)
    ] (const FVector& position) {
            return position.X > minX && position.X < maxX &&
                position.Y > minY && position.Y < maxY &&
                position.Z > minZ && position.Z < maxZ;
        };

        TArray<APlayableCharacter*>& characters = datas.m_playableCharacterArray;

        m_characterOnBox.Empty(characters.Num() + 1);
        for(APlayableCharacter* player : characters)
        {
            if(containPredicate(player->GetActorLocation()))
            {
                m_characterOnBox.Add(player);
            }
        }

        AKing* m_king = Role >= ROLE_Authority ? datas.getServerKing(this) : datas.m_king;
        if(containPredicate(m_king->GetActorLocation()))
        {
            m_characterOnBox.Add(m_king);
        }
}

void ASpawnerTriggerBox::internalSpawn()
{
    UWorld* world = this->GetWorld();

    if(world)
    {
        if(m_random)
        {
            int32 ennemyCountToSpawn = FMath::RandRange(1, m_populationToSpawn.Num());
            int32 lastIndex = m_populationToSpawn.Num() - 1;

            m_spawned.Reserve(ennemyCountToSpawn);
            for(int32 iter = 0; iter < ennemyCountToSpawn; ++iter)
            {
                this->internalSpawnCharacterAtIndex(FMath::RandRange(0, lastIndex), world);
            }
        }
        else
        {
            m_spawned.Reserve(m_populationToSpawn.Num());
            for(int32 iter = 0; iter < m_populationToSpawn.Num(); ++iter)
            {
                this->internalSpawnCharacterAtIndex(iter, world);
            }
        }

        m_spawned.Shrink();
    }
}

void ASpawnerTriggerBox::internalSpawnCharacterAtIndex(int32 index, UWorld* world)
{
    ANonPlayableCharacter* spawned;

    if(m_relativePosition)
    {
        FTransform intermediary = this->GetTransform().GetRelativeTransform(m_populationTransform[index]);

        spawned = Cast<ANonPlayableCharacter>(world->SpawnActor(m_populationToSpawn[index], &intermediary, m_spawnParams));
    }
    else
    {
        spawned = Cast<ANonPlayableCharacter>(world->SpawnActor(m_populationToSpawn[index], &m_populationTransform[index], m_spawnParams));
    }

    if(spawned)
    {
        m_spawned.Add(spawned);

        if (spawned->Controller && m_autoActivateCombat)
        {
             setNearTarget(spawned);
        }

        spawned->spawnEffect();
    }
}

void ASpawnerTriggerBox::spawnEnnemies()
{
    if(Role < ROLE_Authority)
    {
        this->serverSpawnEnnemies();
    }
    else
    {
        this->internalSpawn(); 
    }
}

void ASpawnerTriggerBox::serverSpawnEnnemies_Implementation()
{
    this->internalSpawn();
}

bool ASpawnerTriggerBox::serverSpawnEnnemies_Validate()
{
    return true;
}


void ASpawnerTriggerBox::correctDestruction()
{
    this->Destroy(true);
}

void ASpawnerTriggerBox::killItself()
{
    if(Role < ROLE_Authority)
    {
        this->serverKills();
    }
    else
    {
        this->multiKills();
    }

    this->correctDestruction();
}

void ASpawnerTriggerBox::serverKills_Implementation()
{
    this->multiKills();
    this->correctDestruction();
}

bool ASpawnerTriggerBox::serverKills_Validate()
{
    return true;
}

void ASpawnerTriggerBox::multiKills_Implementation()
{
    this->correctDestruction();
}

void ASpawnerTriggerBox::setNearTarget(ANonPlayableCharacter* spawned)
{
    FVector spawnedLocation = spawned->GetActorLocation();
    float minDist = m_maxDist * m_maxDist;

    ACustomAIControllerBase* controller = Cast<ACustomAIControllerBase>(spawned->Controller);

    if(controller)
    {
        for(ARobotRebellionCharacter* charac : m_characterOnBox)
        {
            float toCheck = spawnedLocation.SizeSquared();
            if(minDist < toCheck)
            {
                minDist = toCheck;
                controller->setTarget(charac);
            }
        }
    }
}