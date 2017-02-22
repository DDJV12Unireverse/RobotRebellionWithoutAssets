// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "RobotRebellionCharacter.h"
#include "OnlineSubsystem.h"
#include "LobbyUIWidget.h"

void ULobbyUIWidget::initialiseOnliSubsystem()
{
    m_onlineSub = IOnlineSubsystem::Get();
    if(!m_onlineSub)
    {
        PRINT_MESSAGE_ON_SCREEN(FColor::Red, TEXT("No OnlineSubsytem found!"))
    }

    /** Bind function for CREATING a Session */
    OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &ULobbyUIWidget::OnCreateSessionComplete);
    OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &ULobbyUIWidget::OnStartSessionComplete);

    /** Bind function for FINDING a Session */
    OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &ULobbyUIWidget::OnFindSessionsComplete);

    /** Bind function for JOINING a Session */
    OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &ULobbyUIWidget::OnJoinSessionComplete);

    /** Bind function for DESTROYING a Session */
    OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &ULobbyUIWidget::OnDestroySessionComplete);
}

void ULobbyUIWidget::CreateServer(FString mapName)
{
    auto currentCharacter = Cast<ARobotRebellionCharacter>(GetOwningPlayer()->GetCharacter());

    FString command = "open " + mapName + "?listen";

    currentCharacter->ExecuteCommand(command);
}

void ULobbyUIWidget::JoinServer(FString IPAdress)
{
    auto currentCharacter = Cast<ARobotRebellionCharacter>(GetOwningPlayer()->GetCharacter());

    FString command = "open " + IPAdress + ":7777";

    currentCharacter->ExecuteCommand(command);
}

bool ULobbyUIWidget::HostSession()
{
    ULocalPlayer* const Player = GetOwningPlayer()->GetLocalPlayer();
    TSharedPtr<const FUniqueNetId> UserId = Player->GetPreferredUniqueNetId();

    // Get the Session Interface, so we can call the "CreateSession" function on it
    IOnlineSessionPtr Sessions = m_onlineSub->GetSessionInterface();

    if(Sessions.IsValid() && UserId.IsValid())
    {
        /*
        Fill in all the Session Settings that we want to use.

        There are more with SessionSettings.Set(...);
        For example the Map or the GameMode/Type.
        */
        SessionSettings = MakeShareable(new FOnlineSessionSettings());

        SessionSettings->bIsLANMatch = IS_LAN;
        SessionSettings->bUsesPresence = USE_PRESENCE;
        SessionSettings->NumPublicConnections = MAX_PLAYERS;
        SessionSettings->NumPrivateConnections = MAX_PLAYERS;
        SessionSettings->bAllowInvites = true;
        SessionSettings->bAllowJoinInProgress = true;
        SessionSettings->bShouldAdvertise = true;
        SessionSettings->bAllowJoinViaPresence = USE_PRESENCE;
        SessionSettings->bAllowJoinViaPresenceFriendsOnly = USE_PRESENCE;

        //SessionSettings->Set(SETTING_MAPNAME, m_openMapName, EOnlineDataAdvertisementType::ViaOnlineService);

        // Set the delegate to the Handle of the SessionInterface
        OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

        // Our delegate should get called when this is complete (doesn't need to be successful!)
        return Sessions->CreateSession(*UserId, m_gameSessionName, *SessionSettings);
    }
    return false;
}

void ULobbyUIWidget::FindSessions()
{
    ULocalPlayer* const Player = GetOwningPlayer()->GetLocalPlayer();
    TSharedPtr<const FUniqueNetId> UserId = Player->GetPreferredUniqueNetId();

    // Get the SessionInterface from our OnlineSubsystem
    IOnlineSessionPtr Sessions = m_onlineSub->GetSessionInterface();

    if(Sessions.IsValid() && UserId.IsValid())
    {
        /*
        Fill in all the SearchSettings, like if we are searching for a LAN game and how many results we want to have!
        */
        SessionSearch = MakeShareable(new FOnlineSessionSearch());


        SessionSearch->bIsLanQuery = IS_LAN;
        SessionSearch->MaxSearchResults = 20;
        SessionSearch->PingBucketSize = 50;

        TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

        // Set the Delegate to the Delegate Handle of the FindSession function
        OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

        // Finally call the SessionInterface function. The Delegate gets called once this is finished
        Sessions->FindSessions(*UserId, SearchSettingsRef);
    }
}

void ULobbyUIWidget::JoinLanSession()
{
    ULocalPlayer* const Player = GetOwningPlayer()->GetLocalPlayer();
    TSharedPtr<const FUniqueNetId> UserId = Player->GetPreferredUniqueNetId();

    // Just a SearchResult where we can save the one we want to use, for the case we find more than one!
    FOnlineSessionSearchResult SearchResult;

    // If the Array is not empty, we can go through it
    if(SessionSearch->SearchResults.Num() > 0)
    {
        for(int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
        {
            // To avoid something crazy, we filter sessions from ourself
            if(SessionSearch->SearchResults[i].Session.OwningUserId != Player->GetPreferredUniqueNetId())
            {
                SearchResult = SessionSearch->SearchResults[i];

                // Once we found sounce a Session that is not ours, just join it. Instead of using a for loop, you could
                // use a widget where you click on and have a reference for the GameSession it represents which you can use
                // here
                IOnlineSessionPtr Sessions = m_onlineSub->GetSessionInterface();

                if(Sessions.IsValid() && UserId.IsValid())
                {
                    // Set the Handle again
                    OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

                    // Call the "JoinSession" Function with the passed "SearchResult". The "SessionSearch->SearchResults" can be used to get such a
                    // "FOnlineSessionSearchResult" and pass it. Pretty straight forward!
                    Sessions->JoinSession(*UserId, m_gameSessionName, SearchResult);
                }
                break;
            }
        }
    }
}

void ULobbyUIWidget::DestroySessionAndLeaveGame()
{
    IOnlineSessionPtr Sessions = m_onlineSub->GetSessionInterface();

    if(Sessions.IsValid())
    {
        Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

        Sessions->DestroySession(m_gameSessionName);
    }
}

void ULobbyUIWidget::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Yellow, TEXT("Create Session finished starting delegate function"));
    FString debugMessage = "OnCreateSessionComplete " + m_gameSessionName.ToString() + " " + FString::FromInt(bWasSuccessful);
    PRINT_MESSAGE_ON_SCREEN(FColor::Red, debugMessage);

    // Get the Session Interface to call the StartSession function
    IOnlineSessionPtr Sessions = m_onlineSub->GetSessionInterface();

    if(Sessions.IsValid())
    {
        // Clear the SessionComplete delegate handle, since we finished this call
        Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
        if(bWasSuccessful)
        {
            // Set the StartSession delegate handle
            OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

            // Our StartSessionComplete delegate should get called after this
            Sessions->StartSession(m_gameSessionName);
        }
    }
}
void ULobbyUIWidget::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
    FString debugMessage = "OnStartSessionComplete " + m_gameSessionName.ToString() + " " + FString::FromInt(bWasSuccessful);
    PRINT_MESSAGE_ON_SCREEN(FColor::Red, debugMessage);

    // Get the Session Interface to clear the Delegate
    IOnlineSessionPtr Sessions = m_onlineSub->GetSessionInterface();
    if(Sessions.IsValid())
    {
        // Clear the delegate, since we are done with this call
        Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
    }


    // If the start was successful, we can open a NewMap if we want. Make sure to use "listen" as a parameter!
    if(bWasSuccessful)
    {
        UGameplayStatics::OpenLevel(GetWorld(), m_openMapName, true, "listen");
    }
}
void ULobbyUIWidget::OnFindSessionsComplete(bool bWasSuccessful)
{
    PRINT_MESSAGE_ON_SCREEN(FColor::Red, FString::Printf(TEXT("OFindSessionsComplete bSuccess: %d"), bWasSuccessful));

    // Get SessionInterface of the OnlineSubsystem
    IOnlineSessionPtr Sessions = m_onlineSub->GetSessionInterface();
    if(Sessions.IsValid())
    {
        // Clear the Delegate handle, since we finished this call
        Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

        // Just debugging the Number of Search results. Can be displayed in UMG or something later on
        PRINT_MESSAGE_ON_SCREEN(FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num()));

        // If we have found at least 1 session, we just going to debug them. You could add them to a list of UMG Widgets, like it is done in the BP version!
        if(SessionSearch->SearchResults.Num() > 0)
        {
            // "SessionSearch->SearchResults" is an Array that contains all the information. You can access the Session in this and get a lot of information.
            // This can be customized later on with your own classes to add more information that can be set and displayed
            for(int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
            {
                // OwningUserName is just the SessionName for now. I guess you can create your own Host Settings class and GameSession Class and add a proper GameServer Name here.
                // This is something you can't do in Blueprint for example!
                // TODO - Afficher la liste des parties sur l'écran (dans l'UI)
                PRINT_MESSAGE_ON_SCREEN(FColor::Yellow, FString::Printf(TEXT("Session Number: %d | Sessionname: %s "), SearchIdx + 1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName)));
            }
        }
    }
}
void ULobbyUIWidget::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    IOnlineSessionPtr Sessions = m_onlineSub->GetSessionInterface();

    if(Sessions.IsValid())
    {
        // Clear the Delegate again
        Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

        // Get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map
        // This is something the Blueprint Node "Join Session" does automatically!
        APlayerController * const PlayerController = GetOwningPlayer();

        // We need a FString to use ClientTravel and we can let the SessionInterface contruct such a
        // String for us by giving him the SessionName and an empty String. We want to do this, because
        // Every OnlineSubsystem uses different TravelURLs
        FString TravelURL;

        if(PlayerController && Sessions->GetResolvedConnectString(m_gameSessionName, TravelURL))
        {
            // Finally call the ClienTravel. If you want, you could print the TravelURL to see
            // how it really looks like
            PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
        }
    }
}
void ULobbyUIWidget::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    // Get the SessionInterface from the OnlineSubsystem
    IOnlineSessionPtr Sessions = m_onlineSub->GetSessionInterface();

    if(Sessions.IsValid())
    {
        // Clear the Delegate
        Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

        // If it was successful, we just load another level (could be a MainMenu!)
        if(bWasSuccessful)
        {
            UGameplayStatics::OpenLevel(GetWorld(), m_mainMenuMapName, true);
        }
    }
}
