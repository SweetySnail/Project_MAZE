#include "Graph.h"
#include "Stack.h"
#include "MinHeap.h"
#include <set>

#define DFS_FIRST_PATH

Graph::Graph()
{
	m_pVHead = NULL;
	m_vSize = 0;
}

Graph::~Graph()
{
	Clear();
}

Vertex* Graph::GetHead() { return m_pVHead; }

void Graph::AddVertex(int vertexKey)
{
	if (m_pVHead == NULL)
	{
		m_pVHead = new Vertex(vertexKey);
		return;
	}
	
	Vertex* ver = new Vertex(vertexKey);
	Vertex* pCur = m_pVHead;
	while (pCur->GetNext())
		pCur = pCur->GetNext();
	pCur->SetNext(ver);
}

void Graph::AddEdge(int startVertexKey, int endVertexKey, int weight)
{
	Vertex* pVer = m_pVHead;
	if (pVer == NULL)
		return;
	while (pVer)
	{
		if (pVer->GetKey() == startVertexKey)
			break;
		else
			pVer = pVer->GetNext();
	}
	if (pVer == NULL)
		return;
	else
	{
		int size = pVer->Size();
		pVer->AddEdge(endVertexKey, weight);
		pVer->SetSize(size + 1);
	}
}

void Graph::SetSize(int size) { m_vSize = size; }

int Graph::Size() const { return m_vSize; }

void Graph::Clear()
{
	delete m_pVHead;
}

void Graph::Print(std::ofstream& fout)
{
	Vertex* pCur = m_pVHead;
	Edge* pEdge;
	fout << "====== PRINT ====== " << endl;
	while (pCur)
	{
		pEdge = pCur->GetHeadOfEdge();
		for (int i = 0; i < m_vSize; i++)
		{
			if (pEdge != NULL && pEdge->GetKey() == i)
			{
				fout << pEdge->GetWeight() << " ";
				pEdge = pEdge->GetNext();
			}
			else
				fout << "0 ";
		}
		fout << endl;
		pCur = pCur->GetNext();
	}
	fout << "===================" << endl;
}

Vertex* Graph::FindVertex(int key)
{
	Vertex* pCur = m_pVHead;
	while (pCur)
	{
		if (pCur->GetKey() == key)
			return pCur;
		pCur = pCur->GetNext();
	}
	return NULL;
}

bool Graph::IsNegativeEdge()
{
	Vertex* pCur = m_pVHead;
	if (pCur == NULL)
		return 0;

	while (pCur)
	{
		Edge* pEdge = pCur->GetHeadOfEdge();
		while (pEdge)
		{
			if (pEdge->GetWeight() < 0)
				return 1;
			pEdge = pEdge->GetNext();
		}
		pCur = pCur->GetNext();
	}
	return 0;
}

std::vector<int> Graph::FindPathDfs(int startVertexKey, int endVertexKey)
{
	vector<int> path;
	bool* visit = new bool[m_vSize];
	Stack<Edge*> s;
	int cost = 0, vKey, fcost = 0;
	int flag = 0, back_flag = 0, no_flag = 0;
	
	// Initialization visit
	for (int i = 0; i < m_vSize; i++)
		visit[i] = false;

	// Set Start Vertex
	Vertex* pCur = FindVertex(startVertexKey);
	visit[startVertexKey] = true;
	path.push_back(startVertexKey);

	// Push Edge in Stack
	Edge* pEdge = pCur->GetHeadOfEdge();
	while (pEdge)
	{
		if (visit[pEdge->GetKey()] != 1)
			s.Push(pEdge);
		pEdge = pEdge->GetNext();
	}
	
	// DFS
	while (1)
	{
		Edge* pVisit = s.Top();
		s.Pop();

		vKey = pVisit->GetKey();
		visit[vKey] = true;
		path.push_back(vKey);
		cost = cost + pVisit->GetWeight();

		// If Reach End Vertex
		if (vKey == endVertexKey)
		{
			path.push_back(cost);
			return path;
		}
		
		pCur = FindVertex(pVisit->GetKey());
		while (1)
		{
			pEdge = pCur->GetHeadOfEdge();
			while (pEdge)
			{
				if (pEdge->GetKey() == fcost && back_flag == 1)
					cost = cost - pEdge->GetWeight();
				if (visit[pEdge->GetKey()] != true && back_flag == 0)
				{
					s.Push(pEdge);
					flag = 1;
				}

				if (visit[pEdge->GetKey()] == 0)
					no_flag = 1;
					
				pEdge = pEdge->GetNext();
			}

			if (no_flag == 1)
				break;
			else if (flag == 0)
			{
				path.pop_back();
				fcost = pCur->GetKey();
				pCur = FindVertex(path.back());
				back_flag = 1;
			}

			if (flag == 1 || back_flag == 0)
				break;
		}
	}
}

std::vector<int> Graph::FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey)
{
	int* distance = new int[m_vSize];
	int* path = new int[m_vSize];
	bool* visit = new bool[m_vSize];
	vector<int> v;
	set <int, less<int>> s;
	int count = 0;

	// Initialize distance
	distance[startVertexKey] = 0;
	for (int i = 0; i < m_vSize; i++)
	{
		if (i == startVertexKey)
			continue;
		distance[i] = IN_FINITY;
	}

	// Initialize path and visit
	for (int i = 0; i < m_vSize; i++)
	{
		path[i] = -1;
		visit[i] = false;
	}

	Vertex* pCur = FindVertex(startVertexKey);
	Vertex* pNext;
	Edge* pEdge = pCur->GetHeadOfEdge();
	while (1)
	{
		pEdge = pCur->GetHeadOfEdge();
		while (pEdge)
		{
			// This is the case when distance is shortened
			if (min(distance[pEdge->GetKey()], distance[pCur->GetKey()] + pEdge->GetWeight()) == distance[pCur->GetKey()] + pEdge->GetWeight())
			{
				// if the distance value alread exists in the set, the corresponding distance value is deleted from the set
				if (s.find(distance[pEdge->GetKey()]) != s.end())
					s.erase(distance[pEdge->GetKey()]);

				// The corresponding key value with visit table false does not push the distance of the corresponding key value into the vector
				if (visit[pEdge->GetKey()] == false)
					v.push_back(distance[pCur->GetKey()] + pEdge->GetWeight());

				// distance and path array update
				distance[pEdge->GetKey()] = distance[pCur->GetKey()] + pEdge->GetWeight();
				path[pEdge->GetKey()] = pCur->GetKey();

			}
			pEdge = pEdge->GetNext();
		}
		// Insert and Delete in Set
		s.insert(v.begin(), v.end());
		s.erase(distance[pCur->GetKey()]);
		if (s.size() == 0)
			break;

		pNext = m_pVHead;
		while (pNext) // Find the key value with the distance
		{
			if (visit[pNext->GetKey()] == false && *s.begin() == distance[pNext->GetKey()])
				break;
			pNext = pNext->GetNext();
		}

		if (count == 0) // visit table of start vertex is true
		{
			visit[startVertexKey] = true;
			count++;
		}
		else // If visit table of all vertex, the function ends
		{
			bool alltrue = true;
			for (int i = 0; i < m_vSize; i++)
			{
				if (visit[i] == false)
					alltrue = false;
			}
			if (alltrue)
				break;
		}
		pCur = FindVertex(pNext->GetKey()); // next vertex
		visit[pCur->GetKey()] = true;
		v.clear(); // vector clear
	}
	v.push_back(distance[endVertexKey]);         // total weight push at vector
	v.push_back(endVertexKey);                   // endvertex push at vector
	while (endVertexKey != startVertexKey)       // endvertex changes continuously
	{
		pCur = FindVertex(path[endVertexKey]);
		endVertexKey = pCur->GetKey();
		v.push_back(endVertexKey);
	}
	return v;
}

std::vector<int> Graph::FindShortestPathDijkstraUsingMinHeap(int startVertexKey, int endVertexKey)
{
	vector<int> route;
	MinHeap<int, int > heap;
	MinHeap<int, int > copy_heap;
	
	int finish_flag = 0;
	int scost, backup_cost;

	// Initialize distance
	int* distance = new int[m_vSize];
	for (int i = 0; i < m_vSize; i++)
	{
		if (i == startVertexKey)
			continue;
		distance[i] = IN_FINITY;
	}

	// Initialize visit & path
	bool* visit = new bool[m_vSize];
	int* path = new int[m_vSize];
	for (int i = 0; i<m_vSize; i++)
	{
		visit[i] = false;
		path[i] = -1;
	}

	Vertex * pCur = FindVertex(startVertexKey);
	Edge * pEdge = pCur->GetHeadOfEdge();;
	visit[startVertexKey] = true;//start vertex is visited.

	while (pEdge)//Push startVertex's edge into heap.
	{
		path[pEdge->GetKey()] = startVertexKey;
		distance[pEdge->GetKey()] = pEdge->GetWeight();
		heap.Push(pEdge->GetWeight(), pEdge->GetKey());
		pEdge = pEdge->GetNext();
	}
	pair<int, int> it;

	while (1)
	{
		finish_flag = 0;//flag for finish.
		while (!heap.IsEmpty())//if heap is empty -> finish.
		{
			it = heap.Top();
			heap.Pop();
			if (visit[it.second] == false)//if vertex isn't visited.
			{
				visit[it.second] = true;//set visit.
				finish_flag = 1;//set flag
				break;//start compare.
			}
		}
		if (finish_flag == 0)
			break;

		backup_cost = it.first;//the minimum weight from this vertex.
		pCur = FindVertex(it.second);//the vertex which isn't visited.
		pEdge = pCur->GetHeadOfEdge();//Store first edge of this vertex.

		while (pEdge)
		{
			if (heap.Get(pEdge->GetKey()).first != -1)//check minimum weight is existed in heap.
			{
				if (heap.Get(pEdge->GetKey()).first > (backup_cost + pEdge->GetWeight()) && visit[pEdge->GetKey()] == false)//case: update.
				{
					path[pEdge->GetKey()] = pCur->GetKey();//adjust path.
					distance[pEdge->GetKey()] = backup_cost + pEdge->GetWeight();//update distance
					heap.DecKey(pEdge->GetKey(), (backup_cost + pEdge->GetWeight()));//insert and sort.
					pEdge = pEdge->GetNext();//move next
					continue;//start next comparing
				}
			}
			else//minimum weight isn't exist in heap.
			{
				if (visit[pEdge->GetKey()] == false)//if visit == 1 -> case: minimum weight is found.
				{
					path[pEdge->GetKey()] = pCur->GetKey();//adjust path
					distance[pEdge->GetKey()] = backup_cost + pEdge->GetWeight();//add weight.
					heap.Push(backup_cost + pEdge->GetWeight(), pEdge->GetKey());//insert
				}
			}
			pEdge = pEdge->GetNext();//move next
		}
	}
	route.push_back(distance[endVertexKey]);
	while (1)//push path in vector
	{
		route.push_back(endVertexKey);
		if (endVertexKey == startVertexKey)
			break;
		endVertexKey = path[endVertexKey];
	}
	return route;
}

std::vector<int>Graph::FindShortestPathBellmanFord(int startVertexKey, int endVertexKey)
{
	vector<int> distance(m_vSize, IN_FINITY);  // save distance vector
	vector<int> c_distance(m_vSize, IN_FINITY);
	vector<int> path(m_vSize, -1);  // save path vector

	// initialize distnace 0
	distance[startVertexKey] = 0;
	c_distance[startVertexKey] = 0;

	// search start vertex
	Vertex* Current_Vertex = FindVertex(startVertexKey);
	Edge* Current_Edge = Current_Vertex->GetHeadOfEdge();

	// repeat until i is smaller than size
	for (int i = 0; i < m_vSize - 1; i++)
	{
		// until Current_edge isn't NULL
		while (Current_Edge != NULL)
		{
			// if new distance can be shorter than before
			if (distance[Current_Edge->GetKey()] > distance[Current_Vertex->GetKey()] + Current_Edge->GetWeight())
			{
				// change distance and path
				distance[Current_Edge->GetKey()] = distance[Current_Vertex->GetKey()] + Current_Edge->GetWeight();
				c_distance[Current_Edge->GetKey()] = c_distance[Current_Vertex->GetKey()] + Current_Edge->GetWeight();
				path[Current_Edge->GetKey()] = Current_Vertex->GetKey();
			}
			Current_Edge = Current_Edge->GetNext();
		}

		// if Current_vertex isn't NULL go next vertex
		if (Current_Vertex->GetNext() != NULL)
			Current_Vertex = Current_Vertex->GetNext();
		// else go first vertex
		else
			Current_Vertex = m_pVHead;
		Current_Edge = Current_Vertex->GetHeadOfEdge();
	}

	Current_Vertex = m_pVHead;
	while (Current_Vertex) // Determine that the negative cycle has occured in the graph
	{
		if (c_distance[Current_Vertex->GetKey()] != IN_FINITY)
		{
			Current_Edge = Current_Vertex->GetHeadOfEdge();
			while (Current_Edge)
			{
				c_distance[Current_Edge->GetKey()] = min(c_distance[Current_Edge->GetKey()], c_distance[Current_Vertex->GetKey()] + Current_Edge->GetWeight());
				// compare_distance and distance array update
				Current_Edge = Current_Edge->GetNext();
			}
		}
		Current_Vertex = Current_Vertex->GetNext();
	}

	// Compare distance vs c_distance
	bool same = true;
	for (int i = 0; i < m_vSize; i++)
	{
		if (distance[i] != c_distance[i])
			same = false;
	}

	// if it can go end vertex,
	// make new vector and save values into new vector
	vector<int> result;
	result.push_back(distance[endVertexKey]);

	int i = endVertexKey;
	result.push_back(i);
	while (path[i] != -1)
	{
		result.push_back(path[i]);
		i = path[i];
	}
	if (same == false)
		result[i] = -10;
	return result;
}

std::vector<vector<int> > Graph::FindShortestPathFloyd()
{
	vector<vector<int>> v(m_vSize, vector<int>(m_vSize, 0));

	// Initialize Array
	for (int i = 0; i < m_vSize; i++)
	{
		for (int j = 0; j < m_vSize; j++)
		{
			v[i][j] = IN_FINITY;
			if(i == j)
				v[i][j] = 0;
		}
	}

	// Set Array
	Vertex* pCur = m_pVHead;
	Edge* pEdge;
	while(pCur != NULL)
	{
		pEdge = pCur->GetHeadOfEdge();
		while (pEdge != NULL)
		{
			v[pCur->GetKey()][pEdge->GetKey()] = pEdge->GetWeight();
			pEdge = pEdge->GetNext();
		}
		pCur = pCur->GetNext();
	}

	// Do Floyd Algorithm
	for (int k = 0; k < m_vSize; k++)
	{
		for (int i = 0; i < m_vSize; i++)
		{
			for (int j = 0; j < m_vSize; j++)
			{
				if (v[i][j] > v[i][k] + v[k][j])
					v[i][j] = v[i][k] + v[k][j];
			}
		}
	}
	return v;
}