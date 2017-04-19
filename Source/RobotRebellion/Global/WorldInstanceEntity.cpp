// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "WorldInstanceEntity.h"


// Sets default values
AWorldInstanceEntity::AWorldInstanceEntity()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWorldInstanceEntity::BeginPlay()
{
	Super::BeginPlay();

    GameAlterationInstaller& installer = GameAlterationInstaller::getInstance();
    installer.installAlteration<UStunAlteration>(&m_stunDefault);
    installer.installAlteration<UInvisibilityAlteration>(&m_invisibleDefault);
    installer.installAlteration<UShieldAlteration>(&m_shieldDefault);

    EntityDataSingleton& datas = EntityDataSingleton::getInstance();
    datas.m_showVolumeBox = this->m_showVolumeBox;
    datas.m_showEnnemyDetectionSphere = this->m_showEnnemyDetectionSphere;

    NavigationVolumeGraph& navGraph = NavigationVolumeGraph::getInstance();
    navGraph.m_showConnection = this->m_showVolumeConnection;
}

// Called every frame
void AWorldInstanceEntity::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    EntityDataSingleton::getInstance().update(this->GetWorld());
}

