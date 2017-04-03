// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotRebellion.h"
#include "Character/RobotRebellionCharacter.h"
#include "Character/PlayableCharacter.h"
#include "OnlineSubsystem.h"
#include "LobbyUIWidget.h"

#include "SessionWidget.h"

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

    m_sessionSearch = MakeShareable(new FOnlineSessionSearch());
    m_sessionsScrollBox = Cast<UScrollBox>(GetWidgetFromName("SessionsScrollBox"));
    if(m_sessionsScrollBox)
    {
        PRINT_MESSAGE_ON_SCREEN(FColor::Black, TEXT("scroll Box gotten"));
    }
}

void ULobbyUIWidget::setSelectedSession(int index)
{
    if(m_selectedSessionIndex == index)
    {
        PRINT_MESSAGE_ON_SCREEN(FColor::Cyan, TEXT("Session already selected"));
        return;
    }

    for(int i = 0; i < m_sessionsScrollBox->GetChildrenCount(); ++i)
    {
        Cast<USessionWidget>(m_sessionsScrollBox->GetChildAt(i))->setSelected(false);
    }
    Cast<USessionWidget>(m_sessionsScrollBox->GetChildAt(index))->setSelected();
    m_selectedSessionIndex = index;
    PRINT_MESSAGE_ON_SCREEN(FColor::Cyan, TEXT("New session selected : " + FString::FromInt(index)));
}

void ULobbyUIWidget::CreateServer(FString mapName)
{
    auto currentCharacter = Cast<APlayableCharacter>(GetOwningPlayer()->GetCharacter());

    FString command = "open " + mapName + "?listen";
    if(currentCharacter)
    {
        currentCharacter->ExecuteCommand(command);        
    }
}

void ULobbyUIWidget::JoinServer(FString IPAdress)
{
    auto currentCharacter = Cast<APlayableCharacter>(GetOwningPlayer()->GetCharacter());

    FString command = "open " + IPAdress + ":7777";

    if(currentCharacter)
    {
        currentCharacter->ExecuteCommand(command);
    }
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

        There are more with m_sessionSettings.Set(...);
        For example the Map or the GameMode/Type.
        */
        m_sessionSettings = MakeShareable(new FOnlineSessionSettings());

        m_sessionSettings->bIsLANMatch = IS_LAN;
        m_sessionSettings->bUsesPresence = USE_PRESENCE;
        m_sessionSettings->NumPublicConnections = MAX_PLAYERS;
        m_sessionSettings->NumPrivateConnections = MAX_PLAYERS;
        m_sessionSettings->bAllowInvites = true;
        m_sessionSettings->bAllowJoinInProgress = true;
        m_sessionSettings->bShouldAdvertise = true;
        m_sessionSettings->bAllowJoinViaPresence = USE_PRESENCE;
        m_sessionSettings->bAllowJoinViaPresenceFriendsOnly = USE_PRESENCE;

        //m_sessionSettings->Set(SETTING_MAPNAME, m_openMapName, EOnlineDataAdvertisementType::ViaOnlineService);

        // Set the delegate to the Handle of the SessionInterface
        OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

        // Our delegate should get called when this is complete (doesn't need to be successful!)
        return Sessions->CreateSession(*UserId, m_gameSessionName, *m_sessionSettings);
    }
    return false;
}

void ULobbyUIWidget::FindSessions()
{
    m_selectedSessionIndex = -1; // initailiaze at -1 to avoid confusion with index 0

    ULocalPlayer* const Player = GetOwningPlayer()->GetLocalPlayer();
    TSharedPtr<const FUniqueNetId> UserId = Player->GetPreferredUniqueNetId();

    // Get the SessionInterface from our OnlineSubsystem
    IOnlineSessionPtr Sessions = m_onlineSub->GetSessionInterface();

    if(Sessions.IsValid() && UserId.IsValid())
    {
        /*
        Fill in all the SearchSettings, like if we are searching for a LAN game and how many results we want to have!
        */
        // m_sessionSearch = MakeShareable(new FOnlineSessionSearch());

        m_sessionSearch->bIsLanQuery = IS_LAN;
        m_sessionSearch->MaxSearchResults = 20;
        m_sessionSearch->PingBucketSize = 50;

        TSharedRef<FOnlineSessionSearch> SearchSettingsRef = m_sessionSearch.ToSharedRef();

        // Set the Delegate to the Delegate Handle of the FindSession function
        OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
        
        // Remove all children of the current list
        m_sessionsScrollBox->ClearChildren();

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
    if(m_sessionSearch->SearchResults.Num() > 0 && m_selectedSessionIndex >= 0)
    {
        // To avoid something crazy, we filter sessions from ourself
        if(m_sessionSearch->SearchResults[m_selectedSessionIndex].Session.OwningUserId != Player->GetPreferredUniqueNetId())
        {
            SearchResult = m_sessionSearch->SearchResults[m_selectedSessionIndex];

            // Once we found sounce a Session that is not ours, just join it. Instead of using a for loop, you could
            // use a widget where you click on and have a reference for the GameSession it represents which you can use
            // here
            IOnlineSessionPtr Sessions = m_onlineSub->GetSessionInterface();

            if(Sessions.IsValid() && UserId.IsValid())
            {
                // Set the Handle again
                OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

                // Call the "JoinSession" Function with the passed "SearchResult". The "m_sessionSearch->SearchResults" can be used to get such a
                // "FOnlineSessionSearchResult" and pass it. Pretty straight forward!
                Sessions->JoinSession(*UserId, m_gameSessionName, SearchResult);
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
        PRINT_MESSAGE_ON_SCREEN(FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), m_sessionSearch->SearchResults.Num()));

        // If we have found at least 1 session, we just going to debug them. You could add them to a list of UMG Widgets, like it is done in the BP version!
        if(m_sessionSearch->SearchResults.Num() > 0)
        {
            // "m_sessionSearch->SearchResults" is an Array that contains all the information. You can access the Session in this and get a lot of information.
            // This can be customized later on with your own classes to add more information that can be set and displayed
            for(int32 SearchIdx = 0; SearchIdx < m_sessionSearch->SearchResults.Num(); SearchIdx++)
            {
                // OwningUserName is just the SessionName for now. I guess you can create your own Host Settings class and GameSession Class and add a proper GameServer Name here.
                // This is something you can't do in Blueprint for example!
                // TODO - Afficher la liste des parties sur l'écran (dans l'UI)
                USessionWidget* temp = NewObject<USessionWidget>(GetTransientPackage(), m_sessionWidgetClass);
                temp->initialiseWidget(SearchIdx, this);
                m_sessionsScrollBox->AddChild(temp);
                PRINT_MESSAGE_ON_SCREEN(FColor::Yellow, FString::Printf(TEXT("Session Number: %d | Sessionname: %s "), SearchIdx + 1, *(m_sessionSearch->SearchResults[SearchIdx].Session.OwningUserName)));
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
