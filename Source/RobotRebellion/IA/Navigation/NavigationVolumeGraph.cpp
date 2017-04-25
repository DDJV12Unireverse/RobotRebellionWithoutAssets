

#include "RobotRebellion.h"
#include "NavigationVolumeGraph.h"
#include "EditorGraphVolume.h"
#include "VolumeIdProvider.h"


struct NodeRecordSingleton
{
    int32 m_id;
    NodeRecordSingleton* m_fromNode; // Storing pointer is easier for path reversing
    float m_costSoFar;
    float m_heuristic;
    //float m_estimatedTotalCost; // store the cost so far + heuristic

    NodeRecordSingleton(int32 id, NodeRecordSingleton* fromNode, float cost, float heuristic)
        : m_id{id}, m_fromNode{fromNode}, m_costSoFar{cost}, m_heuristic{heuristic}
    {}

    float estimatedCost() const
    {
        return m_heuristic + m_costSoFar;
    }

    static bool lessOperator(const NodeRecordSingleton &a, const NodeRecordSingleton &b)
    {
        return a.estimatedCost() < b.estimatedCost();
    }
};

NavigationVolumeGraph::NavigationVolumeGraph()
    : m_edges{},
    m_edgesCosts{},
    m_indexEdgesForNode{},
    m_isBuilt{false},
    m_NodeAmountExpected{184}
{
    // default graph is empty
}

NavigationVolumeGraph::~NavigationVolumeGraph()
{
    clearGraph();
}

void NavigationVolumeGraph::clearGraph()
{
    m_edgesCosts.Empty();
    m_edges.Empty();
    m_indexEdgesForNode.Empty();
    m_nodes.Empty();
    m_isBuilt = false;

    VolumeIdProvider::getInstance().reset();
}

int32 NavigationVolumeGraph::getNodeAmount()
{
    return m_nodes.Num();
}

int32 NavigationVolumeGraph::getEdgeAmount()
{
    return m_edges.Num() / 2;
}

bool NavigationVolumeGraph::isReadyToUse()
{
    return m_isBuilt;
}

void NavigationVolumeGraph::addNode(AEditorGraphVolume *newVolume)
{
    if(newVolume)
    {
        m_nodes.Add(newVolume);
    }
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, FString::FromInt(m_nodes.Num()) 
                            + "/" 
                            + FString::FromInt(m_NodeAmountExpected) + " Nodes");
    if(m_nodes.Num() == m_NodeAmountExpected)
    {
        PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, "BUILD")
        build();
    }
}

void NavigationVolumeGraph::build()
{
    if(m_isBuilt)
    {
        return;
    }
    m_isBuilt = true;
    // TODO sort node arrray by 
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, "building the graphe");

    // Reserve memory for what we can
    m_indexEdgesForNode.Reserve(m_nodes.Num() + 1);
    m_indexEdgesForNode.AddUninitialized(m_nodes.Num() + 1);

    // Sort nodes by id
    sortNodeArray();

    // go through all node and copy every data needed
    m_indexEdgesForNode[0] = 0;
    for(int32 index{}; index < m_nodes.Num(); ++index)
    {
        AEditorGraphVolume* currentVolume = m_nodes[index];
        if(index != currentVolume->getId())
        {
            PRINT_MESSAGE_ON_SCREEN(FColor::Red, "id error. Index should equals node id.");
            return;
        }
        // Maybe just exclude the last one from the loop
        if((index + 1) < m_nodes.Num()) // exclude last node rework to avoid an if for every node
        {
            m_indexEdgesForNode[index + 1] = m_indexEdgesForNode[index] + currentVolume->m_neighbour.Num();
        }

        // Report outgoing edges -> Fill m_edges && Edges_cost
        m_edges.AddUninitialized(currentVolume->m_neighbour.Num());
        m_edgesCosts.AddUninitialized(currentVolume->m_neighbour.Num());
        for(int32 neighbourIndex{}; neighbourIndex < currentVolume->m_neighbour.Num(); ++neighbourIndex)
        {
            // TODO
            m_edges[m_indexEdgesForNode[index] + neighbourIndex] = currentVolume->m_neighbour[neighbourIndex];
            m_edgesCosts[m_indexEdgesForNode[index] + neighbourIndex]
                = FVector::Dist(currentVolume->m_neighbour[neighbourIndex]->GetActorLocation(),
                                currentVolume->GetActorLocation());
        }
    }
    m_indexEdgesForNode[m_nodes.Num()] = m_edges.Num();

    // For test draw the graph - just comment to avoid graph drawing at the start
    drawConnections(m_nodes[0]->GetWorld());
}

void NavigationVolumeGraph::sortNodeArray()
{
    // TODO sort node arrray by 
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, "\t sorting node by id...");
    TArray<AEditorGraphVolume*> temporaryArray;
    temporaryArray.AddUninitialized(m_nodes.Num());
    for(int32 index{}; index < m_nodes.Num(); ++index)
    {
        temporaryArray[(m_nodes[index]->getId())] = m_nodes[index];
    }
    m_nodes = temporaryArray;

    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, "\t sorting node by id...Done");
}

TArray<FVector> NavigationVolumeGraph::processAStar(int32 startId, int32 endId) const
{
    //using struct to store intel
    TArray<NodeRecordSingleton*> openList{};
    openList.Heapify(NodeRecordSingleton::lessOperator);
    TArray<NodeRecordSingleton*> closeList{};

    //init first node
    openList.HeapPush(new NodeRecordSingleton(startId, nullptr, 0.f,
                                              FVector::Dist(m_nodes[startId]->GetActorLocation(), m_nodes[endId]->GetActorLocation())),
                      NodeRecordSingleton::lessOperator);

    NodeRecordSingleton* currentNode{};
    int32 currentNodeId = INDEX_NONE;

    while(openList.Num() > 0)
    {
        //get the most promising node
        openList.HeapPop(currentNode, NodeRecordSingleton::lessOperator);
        currentNodeId = currentNode->m_id;

        if(currentNodeId == endId) // end a star at the first goal occurence
        {
            break;
        }

        // expand to current node neighbour
        int32 startNeighbourIndex = m_indexEdgesForNode[currentNodeId];
        int32 endNeighbourIndex = m_indexEdgesForNode[currentNodeId + 1];
        for(int32 neighbourIndex = startNeighbourIndex; neighbourIndex < endNeighbourIndex; ++neighbourIndex)
        {
            int32 neighbourId = m_edges[neighbourIndex]->getId();

            // Check in close list (if in it just continue cause we dont want the best path)
            if(closeList.ContainsByPredicate(
                [neighbourId](NodeRecordSingleton* a) {return a->m_id == neighbourId; }))
            {
                continue;
            }

            // Check in open list 
            float neighbourCost = currentNode->m_costSoFar + m_edgesCosts[neighbourIndex];
            NodeRecordSingleton**  nodeRecordOpenList = openList.FindByPredicate([neighbourId](NodeRecordSingleton* a) {return a->m_id == neighbourId; });
            if(nodeRecordOpenList)
            {
                if(neighbourCost < (*nodeRecordOpenList)->m_costSoFar)
                {
                    // update cost so far and from id
                    (*nodeRecordOpenList)->m_fromNode = currentNode;
                    (*nodeRecordOpenList)->m_costSoFar = neighbourCost;
                    // Sort the array fast to keep it fast. Issue with predicate when using heapSort
                    openList.Heapify(NodeRecordSingleton::lessOperator);
                }
                continue;
            }

            // Then we know it's a fully unvisited node
            float heuristic{};// TODO - Calcul Heuristic
            heuristic = FVector::Dist(m_nodes[neighbourId]->GetActorLocation(), m_nodes[endId]->GetActorLocation());
            openList.HeapPush(new NodeRecordSingleton(neighbourId, currentNode, neighbourCost, heuristic), NodeRecordSingleton::lessOperator);

        }
        closeList.Emplace(currentNode);
    }

    if(currentNodeId != endId)
    {   // No path found
        return TArray<FVector>{};
    }

    // process the path in a reverse way
    TArray<FVector> path;
    while(currentNode->m_id != startId)
    {
        path.Emplace(m_nodes[currentNode->m_id]->GetActorLocation());
        currentNode = currentNode->m_fromNode;
    }
    //finally push first node ID
    path.Emplace(m_nodes[startId]->GetActorLocation());

    //Clear memory
    for(NodeRecordSingleton* currNode : openList)
    {
        delete currNode;
    }
    for(NodeRecordSingleton* currNode : closeList)
    {
        delete currNode;
    }

    return path;
}

int32 NavigationVolumeGraph::getOverlappingVolumeId(const FVector &point)const
{
    // TODO - Use finally some sort of hierarchical volume cluster to process faster

    // Go through all volume an find the one
    for(int index{}; index < m_nodes.Num(); ++index)
    {
        if(m_nodes[index]->contains(point))
        {
            return m_nodes[index]->getId();
        }
    }

    // No volume found
    return -1;
}

int32 NavigationVolumeGraph::getBelowVolume(FVector& point, float offset) const
{
    float minDist = 9E+16f;
    int32 idVolume{};
    bool volumeFind = false;
    for(int index{}; index < m_nodes.Num(); ++index)
    {
        float dist = m_nodes[index]->isBelow(point);
        if(dist >= 0.f && dist < minDist )
        {
            minDist = dist;
            volumeFind = true;
            idVolume = index;
        }
    }
    if(volumeFind)
    {
        point.Z = m_nodes[idVolume]->GetActorLocation().Z
            + m_nodes[idVolume]->m_box->GetScaledBoxExtent().Z
            - offset;
        return idVolume;
    }
    return -1;
}

int32 NavigationVolumeGraph::getNearestVolume(FVector& point, float offset, bool useCenter) const
{
    float minDist = 9E+16f;
    int32 idVolume{};
    for(int index{}; index < m_nodes.Num(); ++index)
    {
        float dist = FVector::DistSquared(m_nodes[index]->GetActorLocation(), point);
        if(dist < minDist)
        { // if distance is inferior to last min dist just update it
            minDist = dist;
            idVolume = index;
        }
    }
    if(useCenter)
    {
        point = m_nodes[idVolume]->GetActorLocation();
        return idVolume;
    }
    else
    {
        // Cast ray form point to center to get impact point
        FHitResult hitActors(ForceInit);
        FCollisionQueryParams TraceParams(TEXT("SteeringTrace"), true);
        TraceParams.bTraceAsyncScene = true;
        // atm only should only proc on static mesh
        FVector centerLocation = m_nodes[idVolume]->GetActorLocation();
        m_nodes[idVolume]->GetWorld()->LineTraceSingleByChannel(hitActors, point, centerLocation, ECC_GameTraceChannel9, TraceParams);
        DRAW_DEBUG_LINE(m_nodes[idVolume]->GetWorld(), point, centerLocation, FColor::Emerald);

        // get direction and normalize it
        FVector direction = centerLocation - point;
        direction.Normalize();
        point = hitActors.ImpactPoint + (offset * direction);
        return idVolume;
    }
    // find nearest point overllaped in the volume
}


////////////////////////////////////////////////////////////////////////
//                           DEBUG                                    //
////////////////////////////////////////////////////////////////////////
void NavigationVolumeGraph::writeGraph() const
{
    FString temp = "edges cost : ";
    for(int32 i{}; i < m_edgesCosts.Num(); ++i)
    {
        temp += FString::SanitizeFloat(m_edgesCosts[i]);
        temp += " ";
    }
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, temp);
    temp = "edges";
    for(int32 i{}; i < m_edges.Num(); ++i)
    {
        temp += FString::FromInt(m_edges[i]->getId());
        temp += " ";
    }
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, temp);
    temp = "index : ";
    for(int32 i{}; i < m_indexEdgesForNode.Num(); ++i)
    {
        temp += FString::FromInt(m_indexEdgesForNode[i]);
        temp += " ";
    }
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, temp);
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, "   and " + FString::FromInt(m_indexEdgesForNode.Num()) + " Node");
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, "contains : " + FString::FromInt(m_edges.Num()) + " edges.");
    PRINT_MESSAGE_ON_SCREEN(FColor::Emerald, "Writing graph data structure");
}

void NavigationVolumeGraph::drawConnections(const UWorld* world) const
{
    if (m_showConnection)
    {
        // TODO draw all connection (use the edges list for that)
        for(int32 indexOfEdges{}; indexOfEdges < m_indexEdgesForNode.Num() - 1; ++indexOfEdges)
        {
            FVector startPosition = m_nodes[indexOfEdges]->GetActorLocation();
            int32 startIndex = m_indexEdgesForNode[indexOfEdges];
            int32 endIndex;
            if(indexOfEdges + 1 == m_indexEdgesForNode.Num())
            {
                endIndex = m_edges.Num();
            }
            else
            {
                endIndex = m_indexEdgesForNode[indexOfEdges + 1];
            }
            for(int32 edgesIndex = startIndex; edgesIndex < endIndex; ++edgesIndex)
            {
                DrawDebugLine(world, startPosition, m_edges[edgesIndex]->GetActorLocation(),
                    FColor::Emerald, false, 30.f, 0, 5.f);
            }
        }
    }
}