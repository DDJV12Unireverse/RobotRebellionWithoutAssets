// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ScrollBox.h"

#include "LobbyUIWidget.generated.h"

class IOnlineSubsystem;
/**
 *
 */
UCLASS()
class ROBOTREBELLION_API ULobbyUIWidget : public UUserWidget
{
    GENERATED_BODY()
private:
    IOnlineSubsystem* m_onlineSub;
    enum ServerSettings
    {
        MAX_PLAYERS = 4,
        IS_LAN = true,
        USE_PRESENCE = false
    };   

    /*
    Create Session
    */
    TSharedPtr<class FOnlineSessionSettings> m_sessionSettings;
    /* Delegate called when session created */
    FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
    /* Delegate called when session started */
    FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
    /** Handles to registered delegates for creating/starting a session */
    FDelegateHandle OnCreateSessionCompleteDelegateHandle;
    FDelegateHandle OnStartSessionCompleteDelegateHandle;

    /*
    Searching Session
    */
    bool m_isFindSessionDone = false;
    TSharedPtr<class FOnlineSessionSearch> m_sessionSearch;
    /** Delegate for searching for sessions */
    FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
    /** Handle to registered delegate for searching a session */
    FDelegateHandle OnFindSessionsCompleteDelegateHandle;

    /*
    Joining Session
    */
    int m_selectedSessionIndex;
    /** Delegate for joining a session */
    FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
    /** Handle to registered delegate for joining a session */
    FDelegateHandle OnJoinSessionCompleteDelegateHandle;

    /*
    Destroying Session
    */
    /** Delegate for destroying a session */
    FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
    /** Handle to registered delegate for destroying a session */
    FDelegateHandle OnDestroySessionCompleteDelegateHandle;

    FName m_gameSessionName = "RobotRebellionSession";

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyServer | Settings")
        FString m_widgetName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyServer | Settings")
        FName m_openMapName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyServer | Settings")
        FName m_mainMenuMapName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyServer | Settings")
        UScrollBox* m_sessionsScrollBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyServer | Settings")
        TSubclassOf<class USessionWidget> m_sessionWidgetClass;

    /*
     * METHODS
     */

private:
    /*
    * Delegate methods
    */
    virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

    void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

    void OnFindSessionsComplete(bool bWasSuccessful);

    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

    virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

public:
    void initialiseOnliSubsystem();

    void setSelectedSession(int index);

    /*
    Server with command line
    */
    UFUNCTION(BlueprintCallable, Category = "LobbyServer | Command")
        void CreateServer(FString mapName);

    UFUNCTION(BlueprintCallable, Category = "LobbyServer | Command")
        void JoinServer(FString IPAdress);

    /*
    Server with unreal session
    */
    UFUNCTION(BlueprintCallable, Category = "LobbyServer | Session")
        bool HostSession();
    UFUNCTION(BlueprintCallable, Category = "LobbyServer | Session")
        void FindSessions();
    UFUNCTION(BlueprintCallable, Category = "LobbyServer | Session")
        void JoinLanSession();

    UFUNCTION(BlueprintCallable, Category = "LobbyServer | Session")
        void DestroySessionAndLeaveGame();
};
