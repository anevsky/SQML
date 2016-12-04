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

    Class implementation:  Graph (graph.cpp)
    $ver.1.0

    Author:                Alex Nevsky
    Created:               2009/11/21
    Email:                 anwer.man@gmail.com
    WebSite:               http://alexnevsky.com

*/

#include <algorithm>
#include <QtGui>

#include "graph.h"
#include "file.h"

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::clear()
{
    m_Vertices.clear();
    m_Edges.clear();

    m_VerticesDegree.clear();
    m_AdjacentForVertexVertices.clear();
    m_AdjacentForVertexEdges.clear();
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
bool Graph::readFile(const QString &fileName)
{
    QFile fileToRead(fileName);
    fileToRead.open(QIODevice::ReadOnly);

    QString fileDataStr;
    fileDataStr = fileToRead.readAll();
    fileToRead.close();

    QFile fileToWrite("sysFiles/sysSetFromFile.igv");
    fileToWrite.open(QIODevice::WriteOnly);
    QTextStream out(&fileToWrite);
    out << fileName;
    fileToWrite.close();

    return true;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
bool Graph::writeFile(const QString &fileName)
{
    return true;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
bool Graph::writeAllFile(const QString &fileName)
{
    return true;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_Edge Graph::SetEdge(
  const Graph::t_Vertex& input_Vertex1, 
  const Graph::t_Vertex& input_Vertex2,
  const Graph::t_EdgeCost& input_Cost,
  Graph::t_Edge& modify_Edge
) 
{
	std::pair<t_Vertex, t_Vertex> vertices;
	vertices.first = input_Vertex1;
	vertices.second = input_Vertex2;
	modify_Edge.first = vertices;
	
	t_EdgeCost cost(input_Cost);
	modify_Edge.second = cost;
	
	return modify_Edge;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_Vertex& 
Graph::GetEdgeVertexFirst(const Graph::t_Edge& input_Edge) 
{
	return input_Edge.first.first;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_Vertex& 
Graph::GetEdgeVertexSecond(const Graph::t_Edge& input_Edge) 
{
	return input_Edge.first.second;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_EdgeCost& 
Graph::GetEdgeCost(const Graph::t_Edge& input_Edge) 
{
	return input_Edge.second;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::SetFromFile(const std::string input_FileName) {
	File file;
	file.Read(input_FileName);
	File::t_FileData fileData;
	file.GetData(fileData);
	
        if (!m_Vertices.empty()) {
		std::cerr << "Oops!.. Graph already created!" << std::endl;
		return;
	}
	
	std::string data;
	for (File::t_FileData::iterator iter = fileData.begin(); 
		 iter != fileData.end(); ++iter) {
                     if (*iter == '@') {
                         break;
                     }

		if (*iter != '\n') {
			data.push_back(*iter);
		}
		
		if (*iter == '\n' || iter+1 == fileData.end()) {
			const File::t_FileData VERTICES_DELIMITER = "--";
			File::t_FileData::size_type posVerticesDelimiter = 
			data.find(VERTICES_DELIMITER);
			
			const File::t_FileData EDGE_COST_DELIMITER = "$";
			File::t_FileData::size_type posEdgeCostDelimiter = 
			data.find(EDGE_COST_DELIMITER);
			if (posEdgeCostDelimiter == File::t_FileData::npos) {
				posEdgeCostDelimiter = 0;
			} 
			
			t_Vertex 
			vertex1(data, 0, posVerticesDelimiter), 
			vertex2(data, posVerticesDelimiter+2, 
					posEdgeCostDelimiter-posVerticesDelimiter-2);
			
			t_EdgeCost edgeCost;
			if (posEdgeCostDelimiter != 0) {
				File::t_FileData cost(data, posEdgeCostDelimiter+1);
				edgeCost = atof(cost.c_str());
				if (edgeCost < 0) {
					m_NegativeEdges = true;
				}
			} else {
				edgeCost = 1;
			}
			
			data.clear();
			
			// set m_Vertices
			m_Vertices.insert(vertex1);
			m_Vertices.insert(vertex2);
			
			// set m_Edges
			t_Edge edge;
			SetEdge(vertex1, vertex2, edgeCost, edge);
			if (std::count(m_Edges.begin(), m_Edges.end(), edge) == false) {
				m_Edges.push_back(edge);
			}
			
                        /*
			// reverse edge for undirected graph
			if (m_Directed == false && vertex1 != vertex2) {
				t_Edge reverseEdge;
				SetEdge(vertex2, vertex1, edgeCost, reverseEdge);
				if (std::count(m_Edges.begin(), m_Edges.end(), reverseEdge) == false) {
					m_Edges.push_back(reverseEdge);
				}
			}
                        */
		}
	}
	
	EdgesOrdering();
	FindVerticesDegree();
	FindVerticesAdjacent();
	FindEdgesAdjacent();
	FindVerticesEccentricity();
	FindRadius();
	FindCenter();
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::EdgesOrdering(void) 
{
	m_Edges.sort();
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
bool Graph::IsVertexInEdge(
  const Graph::t_Edge& input_Edge, 
  const Graph::t_Vertex& input_Vertex
)
{
	if (GetEdgeVertexFirst(input_Edge) == input_Vertex) {
		return true;
	} else {
		return false;
	}

}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::FindVerticesDegree(void) 
{
	for (
		 std::set<t_Vertex>::iterator vertexIter = m_Vertices.begin(); 
		 vertexIter != m_Vertices.end(); 
		 ++vertexIter
		 )
	{
		t_Degree vertexDegree(0);
		for (
			 std::list<t_Edge>::iterator edgeIter = m_Edges.begin(); 
			 edgeIter != m_Edges.end(); 
			 ++edgeIter
			 ) 
		{
			if (IsVertexInEdge(*edgeIter, *vertexIter)) {
				++vertexDegree;
				} else {
					if (vertexDegree) {
						break;
					}
				}

		}
		m_VerticesDegree[*vertexIter] = vertexDegree;
		vertexDegree = 0;		
	}
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::FindVerticesAdjacent(void) 
{
	std::set<t_Vertex> adjacentVertices;
	for (
		 std::set<t_Vertex>::iterator vertexIter = m_Vertices.begin(); 
		 vertexIter != m_Vertices.end(); 
		 ++vertexIter
		 )
	{
		for (
			 std::list<t_Edge>::iterator edgeIter = m_Edges.begin(); 
			 edgeIter != m_Edges.end(); 
			 ++edgeIter
			 ) 
		{
			if (IsVertexInEdge(*edgeIter, *vertexIter)) {
				t_Edge edge(*edgeIter);
				adjacentVertices.insert(GetEdgeVertexSecond(edge));
			} else {
				if (adjacentVertices.size()) {
					break;
				}
			}
		}
		m_AdjacentForVertexVertices[*vertexIter] = adjacentVertices;
		adjacentVertices.clear();
	}
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::FindEdgesAdjacent(void) 
{
	std::list<t_Edge> adjacentEdges;
	for (
		 std::set<t_Vertex>::iterator vertexIter = m_Vertices.begin(); 
		 vertexIter != m_Vertices.end(); 
		 ++vertexIter
		 )
	{
		for (
			 std::list<t_Edge>::iterator edgeIter = m_Edges.begin(); 
			 edgeIter != m_Edges.end(); 
			 ++edgeIter
			 ) 
		{
			if (IsVertexInEdge(*edgeIter, *vertexIter)) {
				t_Edge edge(*edgeIter);
				adjacentEdges.push_back(edge);
			} else {
				if (adjacentEdges.size()) {
					break;
				}
			}
		}
		m_AdjacentForVertexEdges[*vertexIter] = adjacentEdges;
		adjacentEdges.clear();
	}
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::FindVerticesEccentricity(void)
{
	if (m_NegativeEdges == false) {
		for (
			 std::set<t_Vertex>::const_iterator iter = m_Vertices.begin();
			 iter != m_Vertices.end(); 
			 ++iter
			 ) 
		{
			DijkstraAlgorithm(*iter);
		}
	} else {
                std::cerr << "Oops!.. Unfortunately, Graph::FindVerticesEccentricity(void) \
not implemented in this version of the program for this input data with negative \
edges!" << std::endl;
	}
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::FindRadius(void)
{

	
	t_Radius radius(INFINITY);
	
	for (
		 std::set<t_Vertex>::const_iterator iter = m_Vertices.begin();
		 iter != m_Vertices.end(); 
		 ++iter
		 ) 
	{
		if (GetMaxVertexEccentricity(*iter) < radius) {
			radius = GetMaxVertexEccentricity(*iter);
		}
	}
	
	m_Radius = radius;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_Vertex 
Graph::GetNearestVertex(const t_Vertex& input_Vertex) 
{

	
	std::list<t_Edge> adjacentEdges = GetAdjacentEdges(input_Vertex);
	
	t_EdgeCost minEdgeCost(INFINITY), edgeCost;
	
	t_Vertex nearestVertex;
	
	for (
		 std::list<t_Edge>::const_iterator listIter = adjacentEdges.begin(); 
		 listIter != adjacentEdges.end(); 
		 ++listIter
		 ) 
	{
		edgeCost = GetEdgeCost(*listIter);
		if (edgeCost < minEdgeCost) {
			minEdgeCost = edgeCost;
			nearestVertex = GetEdgeVertexSecond(*listIter);
		}
	}
	
	return nearestVertex;
}

//--------------------------------------------------------------
// Dijkstra's algorithm for the edges without negative weight
//--------------------------------------------------------------
void Graph::DijkstraAlgorithm(const Graph::t_Vertex& input_Vertex) 
{
	size_t someNodesComputed(m_Vertices.size());
	
	do {
		--someNodesComputed;
		
		t_Vertex source = input_Vertex;
		
		std::set<t_Vertex> unVisitedVertices = m_Vertices;
		
		std::map<t_Vertex, t_Eccentricity> vertexEccentricity;
		
		for (
			 std::set<t_Vertex>::iterator iter = unVisitedVertices.begin();
			 iter != unVisitedVertices.end(); 
			++iter
			) 
		{
			t_Eccentricity minEccentricity(INFINITY);
			vertexEccentricity[*iter] = minEccentricity;
		}
		
		vertexEccentricity[source] = 0;
		
		unVisitedVertices.erase(source);
		
		t_Vertex currentVertex(source);
		
		t_Vertex nextVertex;
		
		while (unVisitedVertices.size()) {
			
			if (true) {
				
				std::list<t_Edge> adjacentEdgesForCurrentVertex = 
				GetAdjacentEdges(currentVertex);
				
				std::set<t_Vertex> unUsedVertices;
				
				for (
					 std::list<t_Edge>::iterator iter = 
					 adjacentEdgesForCurrentVertex.begin(); 
					 iter != adjacentEdgesForCurrentVertex.end(); 
					 ++iter
					 ) 
				{
					t_Vertex secondVertex = GetEdgeVertexSecond(*iter);
					
					if (unVisitedVertices.count(secondVertex) == 1) {
						unUsedVertices.insert(secondVertex);
						
						t_Eccentricity currentVertexCost;
						
						currentVertexCost = vertexEccentricity[secondVertex];
						
						if (
							(vertexEccentricity[GetEdgeVertexFirst(*iter)] 
							 + GetEdgeCost(*iter)) < currentVertexCost
							) 
						{
							vertexEccentricity[secondVertex] = 
							(vertexEccentricity[GetEdgeVertexFirst(*iter)] 
							+ GetEdgeCost(*iter));
						}
					}
				}
			}
		
			unVisitedVertices.erase(currentVertex);
			
			if (true) {
				std::map<t_Vertex, t_Eccentricity> ecMap;
				
				ecMap = m_VerticesEccentricity[source];
				
				t_Eccentricity minEccentricity(INFINITY);
				
				for (
					 std::map<t_Vertex, t_Eccentricity>::iterator 
					 ecMapIter = ecMap.begin(); 
					 ecMapIter != ecMap.end(); 
					 ++ecMapIter
					 ) 
				{
					if (
						(unVisitedVertices.count(ecMapIter->first) > 0)
						&& (ecMapIter->second < minEccentricity)
						) 
					{
						minEccentricity = ecMapIter->second;
						nextVertex = ecMapIter->first;
					}
				}
			}
			
			if (currentVertex == nextVertex) {
				break;
			} else {
				currentVertex = nextVertex;
			}
			
		}
		
		SetVertexEccentricity(source, vertexEccentricity);
		
	} while (someNodesComputed);
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::FindCenter(void) 
{
	if (m_Center.known == false) {
		
		for (
			 std::set<t_Vertex>::const_iterator iter = m_Vertices.begin();
			 iter != m_Vertices.end(); 
			 ++iter
			 ) 
		{
			if (GetMaxVertexEccentricity(*iter) == m_Radius) {
				m_Center.vertices.insert(*iter);
			}
		}
		
		for (
			 std::set<t_Vertex>::const_iterator iterV = m_Center.vertices.begin();
			 iterV != m_Center.vertices.end(); 
			 ++iterV
			 ) 
		{
			std::list<t_Edge> adjacentEdges = GetAdjacentEdges(*iterV);
			
			for (
				 std::list<t_Edge>::const_iterator iterE = adjacentEdges.begin(); 
				 iterE != adjacentEdges.end(); 
				 ++iterE
				 ) 
			{
				if (m_Center.vertices.count(GetEdgeVertexSecond(*iterE)) > 0) {
					m_Center.edges.push_back(*iterE);
				}
			}
		}
		
		m_Center.edges.sort();
		
		FindMaxDistanceBetweenCenter();
		
		m_Center.known = true;
	}
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::FindMaxDistanceBetweenCenter(void)
{

	
	t_Distance maxDistance(-INFINITY);
	
	for (
		 std::set<t_Vertex>::const_iterator iterV1 = m_Center.vertices.begin();
		 iterV1 != m_Center.vertices.end(); 
		 ++iterV1
		 ) 
	{
		for (
			 std::set<t_Vertex>::const_iterator iterV2 = m_Center.vertices.begin();
			 iterV2 != m_Center.vertices.end(); 
			 ++iterV2
			 ) 
		{
			if (GetDistanceFromVertexToVertex(*iterV1, *iterV2) > maxDistance) {
				maxDistance = GetDistanceFromVertexToVertex(*iterV1, *iterV2);
			}
		}
	}
	
	m_Center.maxDistanceBetweenCenter = maxDistance;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::SetVertexEccentricity(
  Graph::t_Vertex input_Vertex, 
  std::map<Graph::t_Vertex, Graph::t_Eccentricity> input_VertexEccentricity
)
{	
	m_VerticesEccentricity[input_Vertex] = input_VertexEccentricity;
}



//--------------------------------------------------------------
//
//--------------------------------------------------------------
const std::map<Graph::t_Vertex, Graph::t_Eccentricity>  
Graph::GetVertexEccentricity(const Graph::t_Vertex& input_Vertex) 
{
	if (m_VerticesEccentricity.count(input_Vertex)) {
		return m_VerticesEccentricity[input_Vertex];
	} else {
		std::map<t_Vertex, t_Eccentricity> inCorrect;
		return inCorrect;
	}
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_Eccentricity 
Graph::GetMaxVertexEccentricity(const Graph::t_Vertex& input_Vertex)
{

	
	t_Eccentricity maxEccentricity(-INFINITY);
	
	std::map<t_Vertex, t_Eccentricity> ecMap;
	
	ecMap = m_VerticesEccentricity[input_Vertex];
	
	for (
		 std::map<t_Vertex, t_Eccentricity>::iterator 
		 ecMapIter = ecMap.begin(); 
		 ecMapIter != ecMap.end(); 
		 ++ecMapIter
		 ) 
	{
		if (ecMapIter->second > maxEccentricity) 
		{
			maxEccentricity = ecMapIter->second;
		}
	}
	
	return maxEccentricity;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_Radius Graph::GetRadius(void)
{
	return m_Radius;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_Distance 
Graph::GetDistanceFromVertexToVertex(									  
  const Graph::t_Vertex& input_From, 
  const Graph::t_Vertex& input_To
)
{
	if (m_VerticesEccentricity.count(input_From)) {
		std::map<t_Vertex, t_Eccentricity>
		ecMap = m_VerticesEccentricity[input_From];
		
		return ecMap[input_To];
	} else {
		t_Eccentricity inCorrect;
		return inCorrect;
	}

}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_Path Graph::GetShortestPath(
  const Graph::t_Vertex& input_Vertex1, 
  const Graph::t_Vertex& input_Vertex2
)
{

	
	t_Distance distance = GetDistanceFromVertexToVertex(input_Vertex1, input_Vertex2);
	
	if (
		distance == INFINITY ||
		m_Vertices.count(input_Vertex1) == 0 ||
		m_Vertices.count(input_Vertex2) == 0
	) 
	{
		std::cerr << "Is no way from '" << input_Vertex1 <<
		"' to '" << input_Vertex2 << "'." << std::endl;
		
                t_Path path("Oops!.. No way!");
		return path;
	}
	
	if (input_Vertex1 == input_Vertex2) {
		t_Path path(input_Vertex1);
		return path;
	}
	
	std::list<t_Path> way;
	
	if (true) {
		std::set<Graph::t_Vertex> adjacentVertices = 
		GetAdjacentVertices(input_Vertex2);
		
		way.push_back(input_Vertex2);
		
		--distance;
	}
	
	t_Vertex currentVertex;
	currentVertex = input_Vertex2;
	
	while (distance) {
		std::set<t_Vertex> adjacentVertices = 
		GetAdjacentVertices(currentVertex);
		
		for (
			 std::set<t_Vertex>::const_iterator setIter = 
			 adjacentVertices.begin(); 
			 setIter != adjacentVertices.end(); 
			 ++setIter
			 ) 
		{
			if (GetDistanceFromVertexToVertex(input_Vertex1, *setIter) == distance) {
				way.push_back(*setIter);
				currentVertex = (*setIter);
				break;
			}
		}
		--distance;
	}
	
        t_Path path("");
	
	path += input_Vertex1;
	path += "--";
	
	size_t needDash = way.size()-1;
	
	for(
		std::list<t_Path>::reverse_iterator iter = way.rbegin(); 
		iter != way.rend(); 
		++iter
		) 
	{
		path += *iter;
		if (needDash) {
			--needDash;
			path += "--";
		}
	}
		
	return path;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_Degree 
Graph::GetVertexDegree(const Graph::t_Vertex& input_Vertex) 
{
	std::map< t_Vertex, t_Degree >::iterator mapIter = 
	m_VerticesDegree.find(input_Vertex);
	if (mapIter != m_VerticesDegree.end()) {
		return mapIter->second;
	}
	return -1;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const std::set<Graph::t_Vertex> 
Graph::GetAdjacentVertices(const Graph::t_Vertex& input_Vertex) 
{
	std::map< t_Vertex, std::set<t_Vertex> >::iterator mapIter = 
	m_AdjacentForVertexVertices.find(input_Vertex);
	if (mapIter != m_AdjacentForVertexVertices.end()) {
		return mapIter->second;
	} else {
		std::set<t_Vertex> inCorrect;
		return inCorrect;
	}
	
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const std::list<Graph::t_Edge> 
Graph::GetAdjacentEdges(const Graph::t_Vertex& input_Vertex) 
{
	std::map< t_Vertex, std::list<t_Edge> >::iterator mapIter = 
	m_AdjacentForVertexEdges.find(input_Vertex);
	if (mapIter != m_AdjacentForVertexEdges.end()) {
		return mapIter->second;
	} else {
		std::list<t_Edge> inCorrect;
		return inCorrect;
	}
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::SaveAsHtml(std::string output_FileName)
{
    std::ofstream output_Stream;
    output_Stream.open(output_FileName.c_str());

    output_Stream << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\"><html xmlns=\"http://www.w3.org/1999/xhtml\"><head> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><title>IGV - Intelligent Graph Visualizer</title></head><body><h1 id=\"0\"><font color=\"navy\">IGV - Intelligent Graph Visualizer</font></h1>" << std::endl;

    output_Stream << "<p><a href=\"http://alexnevsky.com/igv/\"><img src=\"http://alexnevsky.com/igv/images/IGV-Logo.jpg\" /></a></p><p>It is generated by the program <a href=\"http://alexnevsky.com/igv/\">´IGVª</a> information about the graph.<br><br>Follow the links below to see details:</p><ul><li><a href=\"#1\">Graph Vertices</a></li><li><a href=\"#2\">Graph Edges</a></li> <li><a href=\"#3\">Center Vertices</a></li><li><a href=\"#4\">Center Edges</a></li><li><a href=\"#5\">Max Distance Between Center</a></li> <li><a href=\"#6\">Vertices Degree</a></li><li><a href=\"#7\">Adjacent Vertices For Vertex</a></li><li><a href=\"#8\">Adjacent Edges For Vertex</a></li><li><a href=\"#9\">Eccentricity</a></li></ul>" << std::endl;

    output_Stream << "<h2 id=\"1\"><font color=\"navy\">Graph Vertices <a href=\"#0\" title=\"Go Up\">^</a></font></h2>" << m_Vertices.size() << " pieces:<br><br>"<< std::endl;
    for (
             std::set<t_Vertex>::iterator iter = m_Vertices.begin();
             iter != m_Vertices.end();
             ++iter
             )
    {
            output_Stream << "<b>" << *iter << "</b>" << ";<br>"
            << std::endl;
    }

    output_Stream << "<h2 id=\"2\"><font color=\"navy\">Graph Edges <a href=\"#0\" title=\"Go Up\">^</a></font></h2>" << m_Edges.size() << " pieces:<br><br>" << std::endl;
    for (
             std::list<t_Edge>::iterator iter = m_Edges.begin();
             iter != m_Edges.end();
             ++iter
             )
    {
            output_Stream << "<b>" << GetEdgeVertexFirst(*iter) << "</b>" << " -- "
            << "<b>" << GetEdgeVertexSecond(*iter) << "</b>"
            << ", cost: $" << GetEdgeCost(*iter) << ";<br>" << std::endl;
    }

    ShowCenterVertices(output_Stream);
    ShowCenterEdges(output_Stream);
    ShowCenterMaxDistance(output_Stream);
    ShowVerticesDegree(output_Stream);
    ShowAdjacentForVertexVertices(output_Stream);
    ShowAdjacentForVertexEdges(output_Stream);
    ShowVerticesEccentricity(output_Stream);

    output_Stream << "<p><a href=\"#0\" title=\"Go Up\">^</a></p>" << std::endl;

    output_Stream << "<br><center><p>Copyright (c) 2010 <a href=\"http://alexnevsky.com\">Alex A. Nevsky</a>. All rights reserved.</p></center></body></html>";

    output_Stream << std::endl;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
template <typename T>
void PrintValues(T beg, T end, std::ostream& out)
{
        //out << "<br>";
        while (beg != end) {
                out << *beg++;
                //if (beg != end) out << ", <br>";
                if (beg != end) out << " ";
        }
        //out << std::endl;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::SaveAllInfo(std::string& output_FileName)
{
    std::ofstream output_Stream;
    output_Stream.open(output_FileName.c_str());

    output_Stream << "Graph Edges (" << m_Edges.size() << " pieces): <br>" << std::endl;
            for (
                     std::list<t_Edge>::iterator iter = m_Edges.begin();
                     iter != m_Edges.end();
                     ++iter
                     )
            {
                    output_Stream << GetEdgeVertexFirst(*iter) << " -- "
                    << GetEdgeVertexSecond(*iter)
                    << ", cost: $" << GetEdgeCost(*iter) << std::endl;
            }
    output_Stream << std::endl;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowAllInfo(std::ostream& output_Stream)
{
        ShowGraph(output_Stream);
        ShowVerticesDegree(output_Stream);
        ShowAdjacentForVertexVertices(output_Stream);
        ShowAdjacentForVertexEdges(output_Stream);
        ShowVerticesEccentricity(output_Stream);
        return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowGraph(std::ostream& output_Stream)
{
        ShowVertices(output_Stream);
        ShowEdges(output_Stream);
        output_Stream << "<b>Graph Radius:</b><br>" << std::endl << GetRadius() <<
        std::endl << "<br>" << "<br>" << std::endl;
        ShowCenterVertices(output_Stream);
        ShowCenterEdges(output_Stream);
        ShowCenterMaxDistance(output_Stream);
        return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowVertices(std::ostream& output_Stream)
{
    PrintValues(m_Vertices.begin(), m_Vertices.end(), output_Stream);
    /*
        output_Stream << "<b>Graph Vertices</b> (" << m_Vertices.size() << " pieces): "
        << std::endl;
        PrintValues(m_Vertices.begin(), m_Vertices.end(), output_Stream);
        output_Stream << "<br>" << "<br>" << std::endl;
    */
        return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowEdges(std::ostream& output_Stream)
{
        //output_Stream << "<b>Graph Edges</b> " << m_Edges.size() << " pieces:" << std::endl;
        for (
                 std::list<t_Edge>::iterator iter = m_Edges.begin();
                 iter != m_Edges.end();
                 ++iter
                 )
        {
                //output_Stream << GetEdgeVertexFirst(*iter) << " -- "
                output_Stream << GetEdgeVertexFirst(*iter) << " "
                << GetEdgeVertexSecond(*iter)
                //<< ", cost: $" << GetEdgeCost(*iter) << "<br>" << std::endl;
                << " " << GetEdgeCost(*iter) << " ";
        }
        //output_Stream << "<br>" << std::endl;
        return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowVerticesDegree(std::ostream& output_Stream)
{
        output_Stream << "<h2 id=\"6\"><font color=\"navy\">Vertices Degree <a href=\"#0\" title=\"Go Up\">^</a></font></h2>" << std::endl;
        for (
                 std::map< t_Vertex, t_Degree >::iterator iter = m_VerticesDegree.begin();
                 iter != m_VerticesDegree.end();
                 ++iter
                 )
        {
                output_Stream << "<b>" << iter->first << ",</b> degree: " <<
                iter->second << "<br>";
        }
        return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowAdjacentForVertexVertices(std::ostream& output_Stream)
{
        output_Stream << "<h2 id=\"7\"><font color=\"navy\">Adjacent Vertices For Vertex <a href=\"#0\" title=\"Go Up\">^</a></font></h2>" << std::endl;
        for (
                 std::map< t_Vertex, std::set<t_Vertex> >::iterator
                 mapIter = m_AdjacentForVertexVertices.begin();
                 mapIter != m_AdjacentForVertexVertices.end();
                 ++mapIter
                 )
        {
                output_Stream << "<b>" << mapIter->first << ",</b> adjacent vertices ("
                << mapIter->second.size() << " pieces): ";
                const std::set<t_Vertex>& vertexSet(mapIter->second);
                for (
                         std::set<t_Vertex>::const_iterator setIter = vertexSet.begin();
                         setIter != vertexSet.end();
                         ++setIter
                         )
                {
                        output_Stream << *setIter << ", ";
                }
                output_Stream << "<br>";
        }
        return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowAdjacentForVertexEdges(std::ostream& output_Stream)
{
        output_Stream << "<h2 id=\"8\"><font color=\"navy\">Adjacent Edges For Vertex <a href=\"#0\" title=\"Go Up\">^</a></font></h2>" << std::endl;
        for (
                 std::map< t_Vertex, std::list<t_Edge> >::iterator
                 mapIter = m_AdjacentForVertexEdges.begin();
                 mapIter != m_AdjacentForVertexEdges.end();
                 ++mapIter
                 )
        {
                output_Stream << "<b>" << mapIter->first << ",</b> adjacent edges ("
                << mapIter->second.size() << " pieces): ";
                const std::list<t_Edge>& edgesList(mapIter->second);
                for (
                         std::list<t_Edge>::const_iterator listIter = edgesList.begin();
                         listIter != edgesList.end();
                         ++listIter
                         )
                {
                        output_Stream << GetEdgeVertexFirst(*listIter) << "--"
                        << GetEdgeVertexSecond(*listIter) << "($"
                        << GetEdgeCost(*listIter) << "), ";
                }
                output_Stream << "<br>";
        }
        return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowCenterVertices(std::ostream& output_Stream)
{
        output_Stream << "<h2 id=\"3\"><font color=\"navy\">Center Vertices <a href=\"#0\" title=\"Go Up\">^</a></font></h2> " << m_Center.vertices.size() << " pieces:<br><br>"
        << std::endl;
        for (
                 std::set<t_Vertex>::iterator iter = m_Center.vertices.begin();
                 iter != m_Center.vertices.end();
                 ++iter
                 )
        {
                output_Stream << "<b>" << *iter << "</b>" << ";<br>";
        }
        return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowCenterEdges(std::ostream& output_Stream)
{
        output_Stream << "<h2 id=\"4\"><font color=\"navy\">Center Edges <a href=\"#0\" title=\"Go Up\">^</a></font></h2> " << m_Center.edges.size() << " pieces:<br><br> "
        << std::endl;
        for (
                 std::list<t_Edge>::iterator iter = m_Center.edges.begin();
                 iter != m_Center.edges.end();
                 ++iter
                 )
        {
                output_Stream << GetEdgeVertexFirst(*iter) << " -- "
                << GetEdgeVertexSecond(*iter) << ", cost: $" << GetEdgeCost(*iter)
                << "<br>";
        }
        return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowCenterMaxDistance(std::ostream& output_Stream)
{
        output_Stream << "<h2 id=\"5\"><font color=\"navy\">Max Distance Between Center <a href=\"#0\" title=\"Go Up\">^</a></font></h2>" << std::endl;

        output_Stream << m_Center.maxDistanceBetweenCenter << "<br>";

        return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowVerticesEccentricity(std::ostream& output_Stream)
{
        output_Stream << "<h2 id=\"9\"><font color=\"navy\">Eccentricity <a href=\"#0\" title=\"Go Up\">^</a></font></h2>" << std::endl;
        for (
                 t_VertexEccentricity::iterator
                 mapIter = m_VerticesEccentricity.begin();
                 mapIter != m_VerticesEccentricity.end();
                 ++mapIter
                 )
        {
                std::map<t_Vertex, t_Eccentricity> ecMap = mapIter->second;

                for (
                         std::map<t_Vertex, t_Eccentricity>::iterator
                         ecMapIter = ecMap.begin();
                         ecMapIter != ecMap.end();
                         ++ecMapIter
                         )
                {
                        output_Stream << "From vertex <b>'" << mapIter->first << "'</b> to vertex <b>'"
                        << ecMapIter->first << "'</b>: " << ecMapIter->second << "<br>";
                }
        }

        return output_Stream;
}
