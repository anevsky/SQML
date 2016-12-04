/*

    SQWADRIX - Simple Page Viewer and Searcher
    Copyright (C) 2010 Alex Nevsky, http://www.alexnevsky.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

    WebSite: http://www.alexnevsky.com
    Email: anwer.man@gmail.com
    Facebook: http://www.facebook.com/alexey.nevsky

*/

/*

    Class:                 Graph (graph.h)
    $ver.1.0

    Author:                Alex Nevsky
    Created:               2009/11/21
    Email:                 anwer.man@gmail.com
    WebSite:               http://alexnevsky.com

*/

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <list>
#include <utility>

class Graph {
public:    
    void clear();
    bool readFile(const QString &fileName);
    bool writeFile(const QString &fileName);
    bool writeAllFile(const QString &fileName);

    typedef std::string t_Vertex;
    typedef double t_EdgeCost;
    typedef std::pair< std::pair<t_Vertex, t_Vertex>, t_EdgeCost > t_Edge;
    typedef int t_Degree;
    typedef double t_Eccentricity;
    typedef double t_Distance;
    typedef double t_Radius;
    typedef std::string t_Path;
    typedef std::map< t_Vertex, std::map<t_Vertex, t_Eccentricity> > t_VertexEccentricity;

    bool m_Directed;
    bool m_NegativeEdges;
    std::set<t_Vertex> m_Vertices;
    std::list<t_Edge> m_Edges;

    std::map< t_Vertex, t_Degree > m_VerticesDegree;
    std::map< t_Vertex, std::set<t_Vertex> > m_AdjacentForVertexVertices;
    std::map< t_Vertex, std::list<t_Edge> > m_AdjacentForVertexEdges;
    t_VertexEccentricity m_VerticesEccentricity;
    t_Radius m_Radius;

    struct Center {
        bool known;
        std::set<t_Vertex> vertices;
        std::list<t_Edge> edges;
        t_Distance maxDistanceBetweenCenter;

        Center() : vertices(), edges(), known(false), maxDistanceBetweenCenter(0) {}
    };

    Center m_Center;

    const t_Edge SetEdge(
            const t_Vertex& input_Vertex1,
            const t_Vertex& input_Vertex2,
            const t_EdgeCost& input_Cost,
            t_Edge& modify_Edge
            );

    const t_Vertex& GetEdgeVertexFirst(const t_Edge& input_Edge);
    const t_Vertex& GetEdgeVertexSecond(const t_Edge& input_Edge);
    const t_EdgeCost& GetEdgeCost(const t_Edge& input_Edge);

private:
    void EdgesOrdering(void);

    void DijkstraAlgorithm(const t_Vertex& input_Vertex);

    void FindVerticesDegree(void);
    void FindVerticesAdjacent(void);
    void FindEdgesAdjacent(void);
    void FindVerticesEccentricity(void);

    void FindRadius(void);

    void FindCenter(void);
    void FindMaxDistanceBetweenCenter(void);

    void SetVertexEccentricity(
            t_Vertex input_Vertex,
            std::map<t_Vertex, t_Eccentricity> input_VertexEccentricity
            );

    bool IsVertexInEdge(
            const t_Edge& input_Edge,
            const t_Vertex& input_Vertex
            );

public:
    Graph() : m_Vertices(), m_Edges(), m_Directed(false), m_NegativeEdges(false) {}

    void SetFromFile(const std::string input_FileName);

    const t_Path GetShortestPath(
            const t_Vertex& input_Vertex1,
            const t_Vertex& input_Vertex2
            );

    const t_Distance
            GetDistanceFromVertexToVertex(
                    const t_Vertex& input_From,
                    const t_Vertex& input_To
                    );

    const t_Vertex GetNearestVertex(const t_Vertex& input_Vertex);

    const t_Radius GetRadius(void);

    const t_Degree GetVertexDegree(const t_Vertex& input_Vertex);
    const std::set<t_Vertex> GetAdjacentVertices(const t_Vertex& input_Vertex);
    const std::list<t_Edge> GetAdjacentEdges(const t_Vertex& input_Vertex);

    const std::map<t_Vertex, t_Eccentricity>
            GetVertexEccentricity(const t_Vertex& input_Vertex);

    const t_Eccentricity GetMaxVertexEccentricity(const t_Vertex& input_Vertex);

    void SaveAsHtml(std::string output_FileName);

    void SaveAllInfo(std::string& output_FileName);
    std::ostream& ShowAllInfo(std::ostream& output_Stream);
    std::ostream& ShowGraph(std::ostream& output_Stream);
    std::ostream& ShowVertices(std::ostream& output_Stream);
    std::ostream& ShowEdges(std::ostream& output_Stream);
    std::ostream& ShowVerticesDegree(std::ostream& output_Stream);
    std::ostream& ShowAdjacentForVertexVertices(std::ostream& output_Stream);
    std::ostream& ShowAdjacentForVertexEdges(std::ostream& output_Stream);
    std::ostream& ShowCenterVertices(std::ostream& output_Stream);
    std::ostream& ShowCenterEdges(std::ostream& output_Stream);
    std::ostream& ShowCenterMaxDistance(std::ostream& output_Stream);
    std::ostream& ShowVerticesEccentricity(std::ostream& output_Stream);
};

#endif
