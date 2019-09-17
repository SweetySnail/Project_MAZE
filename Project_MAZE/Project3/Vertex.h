#ifndef VERTEX_H
#define VERTEX_H

#ifndef NULL
#define NULL	0
#endif

#include "Edge.h"

class Vertex
{
private:
    // the key of this vertex
    int m_key;
    // the number of the edges from this vertex to others
    int m_size;
    // the head pointer for the linked list of the edges
    Edge* m_pEHead;
    // the next pointer for the linked list of the vertics
    Vertex* m_pNext;

public:
	Vertex()
	{
		m_key = 0;
		m_size = 0;
		m_pEHead = NULL;
		m_pNext = NULL;
	}
	Vertex(int key)
	{
		m_key = key;
		m_size = 0;
		m_pEHead = NULL;
		m_pNext = NULL;
	}
	~Vertex() {}

	void SetNext(Vertex* pNext) { m_pNext = pNext; }
	int GetKey() const { return m_key; }
	Vertex* GetNext() const { return m_pNext; }
	void SetSize(int size) { m_size = size; }
	int Size() const { return m_size; }

	void AddEdge(int edgeKey, int weight)
	{
		Edge* ed = new Edge(edgeKey, weight);
		if (m_pEHead == NULL)
		{
			m_pEHead = ed;
			return;
		}
		else
		{
			Edge* pCur = m_pEHead;
			while (pCur->GetNext() != NULL)
				pCur = pCur->GetNext();

			pCur->SetNext(ed);
		}
	}

	Edge* GetHeadOfEdge() const { return m_pEHead; }
	void Clear()
	{
		Edge * pCur;
		while (m_pEHead != NULL)
		{
			pCur = m_pEHead;
			m_pEHead = m_pEHead->GetNext();
			delete(pCur);
		}
	}
};

#endif
