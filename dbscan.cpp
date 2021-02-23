#include "dbscan.h"

int DBSCAN::run()
{
    int clusterID = 1;
    for ( const Point  &element : m_points )
    {
        if ( element.clusterID == UNCLASSIFIED )
        {
            if ( expandCluster(element, clusterID) != FAILURE )
            {
                clusterID += 1;
            }
        }
    }

    return 0;
}

int DBSCAN::expandCluster(Point point, int clusterID)
{    
    vector<int> clusterSeeds = calculateCluster(point);

    if ( clusterSeeds.size() < m_minPoints )
    {
        point.clusterID = NOISE;
        return FAILURE;
    }
    else
    {
        int index = 0;
        int indexCorePoint = 0;
        for( const int  &seed : clusterSeeds)
        {
            m_points.at(seed).clusterID = clusterID;
            if (m_points.at(seed).x == point.x)
            {
                indexCorePoint = index;
            }
            ++index;
        }
        clusterSeeds.erase(clusterSeeds.begin()+indexCorePoint);

        for( vector<int>::size_type i = 0, n = clusterSeeds.size(); i < n; ++i )
        {
            vector<int> clusterNeighors = calculateCluster(m_points.at(clusterSeeds[i]));

            if ( clusterNeighors.size() >= m_minPoints )
            {
                for (  const int  &neighor : clusterNeighors )
                {
                    if ( m_points.at(neighor).clusterID == UNCLASSIFIED || m_points.at(neighor).clusterID == NOISE )
                    {
                        if ( m_points.at(neighor).clusterID == UNCLASSIFIED )
                        {
                            clusterSeeds.push_back(neighor);
                            n = clusterSeeds.size();
                        }
                        m_points.at(neighor).clusterID = clusterID;
                    }
                }
            }
        }

        return SUCCESS;
    }
}

vector<int> DBSCAN::calculateCluster(Point point)
{
    int index = 0;
    vector<int> clusterIndex;
    for ( const Point  &element : m_points )
    {
        if ( calculateDistance(point, element) <= m_epsilon )
        {
            clusterIndex.push_back(index);
        }
        index++;
    }
    return clusterIndex;
}


vector<vector<Point>> DBSCAN::getClusters()
{

    vector<vector<Point>> clusterIndex;
    for ( const Point  &element : m_points )
    {
    	clusterIndex[element.clusterID].push_back(element);
    }
    return clusterIndex;
}

inline double DBSCAN::calculateDistance( Point pointCore, Point pointTarget )
{
    return abs(pointCore.x - pointTarget.x);
}


