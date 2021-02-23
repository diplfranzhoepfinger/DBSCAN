#include "dbscan.h"

int DBSCAN::run()
{
    int clusterID = 0;
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
            vector<int> clusterNeighbors = calculateCluster(m_points.at(clusterSeeds[i]));

            if ( clusterNeighbors.size() >= m_minPoints )
            {
                for (  const int  &neighbor : clusterNeighbors )
                {
                    if ( m_points.at(neighbor).clusterID == UNCLASSIFIED || m_points.at(neighbor).clusterID == NOISE )
                    {
                        if ( m_points.at(neighbor).clusterID == UNCLASSIFIED )
                        {
                            clusterSeeds.push_back(neighbor);
                            n = clusterSeeds.size();
                        }
                        m_points.at(neighbor).clusterID = clusterID;
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
    	if (element.clusterID >= 0) //avoid -1 and -2 Clusters to be used.
    	{
			const size_t clusterId = element.clusterID;
			while (clusterIndex.size() <= clusterId)
			{
				vector<Point> v;
				clusterIndex.push_back(v);
			}
    	clusterIndex[element.clusterID].push_back(element);
    	}
    }
    return clusterIndex;
}


Point DBSCAN::getCenter(vector<Point> vp)
{
	Point pc;
	pc.x = 0;
	pc.clusterID = UNCLASSIFIED;
	if(vp.size() > 0)
	{
		pc.clusterID = vp[0].clusterID;
		for ( const Point  &p : vp )
		{
	    	pc.x += p.x;
	    }
	    pc.x /= vp.size(); //avoid division by 0.
	}
	return pc;
}

inline double DBSCAN::calculateDistance( Point pointCore, Point pointTarget )
{
    return abs(pointCore.x - pointTarget.x);
}


