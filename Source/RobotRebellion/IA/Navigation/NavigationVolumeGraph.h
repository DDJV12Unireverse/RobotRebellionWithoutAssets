#pragma once

#include "../../Tool/IsSingleton.h"
#include "RobotRebellion.h"

/**
*
*/
class ROBOTREBELLION_API NavigationVolumeGraph : private IsSingleton<NavigationVolumeGraph>
{
    GENERATED_USING_FROM_IsSingleton(NavigationVolumeGraph);

private:
    NavigationVolumeGraph();
    
public:
        TArray<class AEditorGraphVolume*> m_edges; // stores every out edges for every vertices size : 2m
        TArray<float> m_edgesCosts; // Stores every cost for every edges size : 2m
        TArray<int32> m_indexEdgesForNode; // Store where edges begin(into previous array) for every vertice size : n
        TArray<AEditorGraphVolume*> m_nodes; // used to store all node in order to build the graph
        bool m_isBuilt;
        int32 m_NodeAmountExpected;
        bool m_showConnection;

public:

    ~NavigationVolumeGraph();

    // Add a new node to the graph
    void addNode(class AEditorGraphVolume* newNode);

    // Build the graph from m_nodes.
    void build();

    // Execute A* between the start node and the end node.
    // Return every waypoint (including start point) if path found
    //    if not return an empty array
    // TODO - when intergrate it in the project : take a ref to a TArray instead of returning new TArray
    TArray<FVector> processAStar(int32 startId, int32 endId) const;

    // Return the id of the volume that contains the given point
    int32 getOverlappingVolumeId(const FVector &point) const;

    // Return the id of the volum below the point and update the point in order to be overlapped by volume
    int32 getBelowVolume(FVector& point, float offset) const;

    // Return the id of the nearest volume and set the point to the found volume center
    int32 getNearestVolume(FVector& point, float offset, bool useCenter = true) const;

    int32 getNodeAmount();
    int32 getEdgeAmount();
    bool isReadyToUse();

    // Debug function to write basics intels on the graph
    void writeGraph() const;
    //Debug Function to show all connections
    void drawConnections(const UWorld* world) const;

    //empty the graph from everything
    void clearGraph();

private:
    // sort node by id, change m_nodes array
    void sortNodeArray();
};
