#include "Manager.h"
#include <cstring>
#include <stdlib.h>
int load_flag = 0;

Manager::~Manager()
{
    if (fout.is_open())
        fout.close();

    if (ferr.is_open())
        ferr.close();
}

void Manager::Run(const char* filepath)
{
    fout.open(RESULT_LOG_PATH);
    ferr.open(ERROR_LOG_PATH);

	ifstream is;
	Result re;
	char str[50] = {};
	char* stok, *seartok;
	is.open(filepath);
	
	if (is.fail())
	{
		re = CommandFileNotExist;
		fout << "CommandFileNotExist"  << endl;
		PrintError(re);
		return;
	}
	
	while (!is.eof())
	{
		is.getline(str, 50);
		stok = strtok(str, " \r");

		if (stok == NULL)
			continue;
		if (!strcmp(stok, "LOAD"))
		{
			stok = '\0';
			stok = strtok(NULL, "\n\r");
			
			re = Load(stok);
			fout << "====== LOAD ======" << endl;
			if (re == 0)
				fout << "Success" << endl;
			else if (re == 101)
				fout << "LoadFileNotExist" << endl;
			fout << "==================" << endl;
			PrintError(re);
		}
		else if (!strcmp(stok, "PRINT"))
		{
			re = Print();
			if (re == 202)
			{
				fout << "GraphNotExist" << endl;
				PrintError(re);
				continue;
			}
			PrintError(re);
		}
		else if (!strcmp(stok, "DFS"))
		{
			stok = strtok(NULL, " ");
			seartok = strtok(NULL, " ");
			if (stok == NULL || seartok == NULL)
			{
				re = VertexKeyNotExist;
				fout << "====== DFS ======" << endl;
				fout << "VertexKeyNotExist" << endl;
				fout << "===================" << endl;
				PrintError(re);
				continue;
			}
			int a = atoi(stok);
			int b = atoi(seartok);
			re = FindPathDfs(a, b);
			if (re != 0)
			{
				fout << "====== DFS ======" << endl;
				if (re == 201)
					fout << "InvalidVertexKey" << endl;
				else if (re == 202)
					fout << "GraphNotExist" << endl;
				else if (re == 203)
					fout << "InvalidAlgorithm" << endl;
				fout << "===================" << endl;
			}
			PrintError(re);
		}
		else if (!strcmp(stok, "DIJKSTRA"))
		{
			stok = strtok(NULL, " ");
			seartok = strtok(NULL, " ");
			if (stok == NULL || seartok == NULL)//error
			{
				re = VertexKeyNotExist;
				PrintError(re);
				fout << "====== DIJKSTRA ======" << endl;
				fout << "VertexKeyNotExist" << endl;
				fout << "======================" << endl;
				continue;
			}
			int a = atoi(stok);
			int b = atoi(seartok);
			re = FindShortestPathDijkstraUsingSet(a, b);
			if (re != 0)//error
			{
				fout << "====== DIJKSTRA ======" << endl;
				if (re == 201)//InvalidVertexKey
				{
					fout << "InvalidVertexKey" << endl;
				}
				else if (re == 202)//GraphNotExist
				{
					fout << "GraphNotExist" << endl;
				}
				else if (re == 203)//InvalidAlgorithm
				{
					fout << "InvalidAlgorithm" << endl;
				}
				fout << "======================" << endl;
			}
			PrintError(re);
		}
		else if (!strcmp(stok, "DIJKSTRAMIN"))
		{
			stok = strtok(NULL, " ");
			seartok = strtok(NULL, " ");

			// If Error
			if (stok == NULL || seartok == NULL)
			{
				re = VertexKeyNotExist;
				PrintError(re);
				fout << "====== DIJKSTRAMIN ======" << endl;
				fout << "VertexKeyNotExist" << endl;
				fout << "=========================" << endl;
				continue;
			}
			int a = atoi(stok);
			int b = atoi(seartok);
			re = FindShortestPathDijkstraUsingMinHeap(a, b);
			
			// If Error
			if (re != 0)
			{ 
				fout << "====== DIJKSTRAMIN ======" << endl;
				if (re == 201)  //InvalidVertexKey
					fout << "InvalidVertexKey" << endl;
				else if (re == 202)  //GraphNotExist
					fout << "GraphNotExist" << endl;
				else if (re == 203)  //InvalidAlgorithm
					fout << "InvalidAlgorithm" << endl;
				fout << "=========================" << endl;
			}
			PrintError(re);
		}
		else if (!strcmp(stok, "BELLMANFORD"))
		{
			stok = strtok(NULL, " ");
			seartok = strtok(NULL, " ");
			if (stok == NULL || seartok == NULL)//error
			{
				re = VertexKeyNotExist;
				PrintError(re);
				fout << "====== BELLMANFORD ======" << endl;
				fout << "VertexKeyNotExist" << endl;
				fout << "=========================" << endl;
				continue;
			}
			int a = atoi(stok);
			int b = atoi(seartok);
			re = FindShortestPathBellmanFord(a, b);
			if (re != 0)//error
			{
				fout << "====== BELLMANFORD ======" << endl;
				if (re == 201)//InvalidVertexKey
					fout << "InvalidVertexKey" << endl;
				else if (re == 202)//GraphNotExist
					fout << "GraphNotExist" << endl;
				else if (re == 204)//InvalidAlgorithm
					fout << "NegativeCycleDetected" << endl;
				fout << "========================" << endl;
			}
			PrintError(re);
		}
		else if (!strcmp(stok, "FLOYD"))
		{
			re = FindShortestPathFloyd();
			if (re != 0)//error
			{
				fout << "====== FLOYD ======" << endl;
				if (re == 202)//GraphNotExist
					fout << "GraphNotExist" << endl;
				fout << "========================" << endl;
			}
			PrintError(re);
		}
		else
		{
			re = NonDefinedCommand;
			fout << "====== " << stok << " ======" << endl;
			fout << "NonDefinedCommand" << endl;
			fout << "====================" << endl;
			PrintError(re);
		}
	}
}

void Manager::PrintError(Result result)
{
    ferr << "Error code: " << result << std::endl;
}

Result Manager::Load(const char* filepath)
{
	int start = 0, end = 0, weight = 0;
	char str[60];
	char* stok;
	Result re_err;

	ifstream is(filepath);
	if (is.fail())
	{
		re_err = LoadFileNotExist;
		return re_err;
	}
	is.getline(str, sizeof(str));
	m_graph.SetSize(atoi(str));
	
	while (!is.eof())
	{
		is.getline(str, sizeof(str));
		if (is.eof())
			break;
		m_graph.AddVertex(start);
		stok = strtok(str, " ");
		while (1)
		{
			if (stok == NULL)
				break;
			weight = atoi(stok);
			if (weight != 0)
				m_graph.AddEdge(start, end, weight);
			end++;
			stok = strtok(NULL, " ");
		}
		start++;
		end = 0;
	}
	load_flag = 1;
	re_err = Success;
	return re_err;
}

Result Manager::Print()
{
	Result re_err;
	if (load_flag == 0)
	{
		re_err = GraphNotExist;
		return re_err;
	}
	m_graph.Print(fout);
	re_err = Success;
	return re_err;
}

Result Manager::FindPathDfs(int startVertexKey, int endVertexKey)
{
	Result re_err;
	vector<int> v;
	vector<int> s;
	
	if (load_flag == 0)
	{
		re_err = GraphNotExist;
		return re_err;
	}
	if (m_graph.IsNegativeEdge() == 1)
	{
		re_err = InvalidAlgorithm;
		return re_err;
	}
	if (m_graph.FindVertex(startVertexKey) == NULL || m_graph.FindVertex(endVertexKey) == NULL)
	{
		re_err = InvalidVertexKey;
		return re_err;
	}

	v = m_graph.FindPathDfs(startVertexKey, endVertexKey);
	s = v;
	s.pop_back();

	fout << "====== DFS ======" << endl;
	fout << "shortest path: ";
	for (int i = 0; i < v.size() - 1; i++)
		fout << v[i] << " ";
	fout << endl;

	fout << "sorted nodes: ";
	QuickSort(s, 0, s.size() - 1);
	for (int i = 0; i < s.size(); i++)
		fout << s[i] << " ";
	fout << endl;

	fout << "path length: ";
	fout << v[v.size() - 1] << endl;
	fout << "=================" << endl;
	re_err = Success;
	return re_err;
}

Result Manager::FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey)
{
	vector<int> v, s;
	Result re_err;

	if (load_flag == 0)
	{
		re_err = GraphNotExist;
		return re_err;
	}
	if (m_graph.IsNegativeEdge() == 1)
	{
		re_err = InvalidAlgorithm;
		return re_err;
	}
	if (m_graph.FindVertex(startVertexKey) == NULL || m_graph.FindVertex(endVertexKey) == NULL)
	{
		re_err = InvalidVertexKey;
		return re_err;
	}

	v = m_graph.FindShortestPathDijkstraUsingSet(startVertexKey, endVertexKey);
	for (int i = 1; i < v.size(); i++)
		s.push_back(v[i]);
	
	fout << "===== DIJKSTRA =====" << endl;
	fout << "shortest path: ";
	for (int i = (v.size() - 1); i > 0; i--)
		fout << v[i] << " ";
	fout << endl;

	fout << "sorted nodes: ";
	QuickSort(s, 0, s.size() - 1);
	for (int i = 0; i < s.size(); i++)
		fout << s[i] << " ";
	fout << endl;

	fout << "path length: " << v[0] << endl;
	fout << "================" << endl;
	
	re_err = Success;
	return re_err;
}

Result Manager::FindShortestPathDijkstraUsingMinHeap(int startVertexKey, int endVertexKey)
{
	vector<int> v, s;
	Result re_err;

	// load isn't executed
	if (load_flag == 0)
	{
		re_err = GraphNotExist;
		return re_err;
	}
	// negative weight
	if (m_graph.IsNegativeEdge() == 1)
	{
		re_err = InvalidAlgorithm;
		return re_err;
	}
	// InvalidVertexKey
	if (m_graph.FindVertex(startVertexKey) == NULL || m_graph.FindVertex(endVertexKey) == NULL)
	{
		re_err = InvalidVertexKey;
		return re_err;
	}

	// Do Dijkstra Using Min Heap
	v = m_graph.FindShortestPathDijkstraUsingMinHeap(startVertexKey, endVertexKey);
	for (int i = 1; i < v.size(); i++)
		s.push_back(v[i]);

	// Print
	fout << "====== DIJKSTRAMIN ======" << endl;
	fout << "shortest path: ";
	for (int i = (v.size() - 1); i > 0; i--)
		fout << v[i] << " ";
	fout << endl;

	fout << "sorted nodes: ";
	QuickSort(s, 0, s.size() - 1);
	for (int i = 0; i < s.size(); i++)
		fout << s[i] << " ";
	fout << endl;

	fout << "path length: " << v[0] << endl;
	fout << "=========================" << endl;
	re_err = Success;
	return re_err;
}

Result Manager::FindShortestPathBellmanFord(int startVertexKey, int endVertexKey)
{
	vector<int> v, s;
	Result re_err;
	
	if (load_flag == 0)
	{
		re_err = GraphNotExist;
		return re_err;
	}
	if (m_graph.FindVertex(startVertexKey) == NULL || m_graph.FindVertex(endVertexKey) == NULL)
	{
		re_err = InvalidAlgorithm;
		return re_err;
	}

	v = m_graph.FindShortestPathBellmanFord(startVertexKey, endVertexKey);
	s = v;
	
	// If Negative Cycle
	if (v[0] == -10)
	{
		re_err = NegativeCycleDetected;
		return re_err;
	}
	else
	{
		fout << "===== BELLMANFORD =====" << endl;
		fout << "shortest path: ";
		for (int i = v.size() - 1; i > 0; i--)
			fout << v[i] << " ";
		fout << endl;

		fout << "sorted nodes: ";
		QuickSort(s, 0, s.size() - 1);
		for (int i = 0; i < s.size() - 1; i++)
			fout << s[i] << " ";
		fout << endl;

		fout << "path length: ";
		fout << v[0] << endl;
		fout << "================" << endl;
	}
	re_err = GraphNotExist;
	return re_err;
}

Result Manager::FindShortestPathFloyd()
{
	vector<vector<int>> v;
	Result re_err;

	if (load_flag == 0)
	{
		re_err = GraphNotExist;
		return re_err;
	}
	
	v = m_graph.FindShortestPathFloyd();

	fout << "===== FLOYD =====" << endl;
	for (int i = 0; i < v.size(); i++)
	{
		for (int j = 0; j < v.size(); j++)
			fout << v[i][j] << " ";
		fout << endl;
	}
	fout << "================" << endl;

	re_err = Success;
	return re_err;
}

void Manager::QuickSort(vector<int>& arr, int low, int high)
{
	
	if (low < high)
	{
		if (high - low + 1 <= 6)
			InsertionSort(arr, low, high);
		else
		{
			int pivot = Partition(arr, low, high);
			QuickSort(arr, low, pivot - 1);
			QuickSort(arr, pivot + 1, high);
		}
	}
}

void Manager::InsertionSort(vector<int>& arr, int low, int high)
{
	for (int i = low + 1; i <= high; i++)
	{
		int key = arr[i];
		int k = i - 1;
		while (k >= 0 && key < arr[k])
		{
			arr[k + 1] = arr[k];
			k--;
		}
		arr[k + 1] = key;
	}
}

int Manager::Partition(vector<int>& arr, int left, int right)
{
	int key = arr[left];
	int i, j;
	for (i = left + 1, j = right; 1;)
	{
		while (i - j > 1 || arr[i] <= key)  left++;
		while (i - j > 1 || arr[j] > key)  right++;
		if (j == i)
		{                                   // 루프 종료 조건 체크
			if (arr[i] > key) i--;           // 키 값 넣을 곳을 결정
			break;
		}
		int tmp = arr[right];        //  바꾸기
		arr[right] = arr[left];
		arr[left] = tmp;
	}
	arr[left] = arr[i];      //  키값을 알맞는 곳에 넣음
	arr[i] = key;
	return i;                 // 키값(중간값)이 들어간 위치를 리턴함
}