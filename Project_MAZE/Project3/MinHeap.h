#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <utility>
#include <vector>
using namespace std;
template<typename TKey, typename TValue>
class MinHeap
{
private:
    // array for the elements which should be heap-sorted
    std::vector<std::pair<TKey, TValue>> m_vec;

public:
	MinHeap() { m_vec.push_back(make_pair(-1, -1)); }//we don't use array's first index.So set (-1,-1) to the heap's first index.	}
    void Push(TKey key, TValue value)
	{
		m_vec.push_back(make_pair(key, value));//Sort in Heapify function.
		Heapify(0);
	}
    void Pop()
	{
		Heapify(1);//Pop the data in Heapify function
	}

    std::pair<TKey, TValue> Top()
	{
		if (IsEmpty())//if heap is empty
			return m_vec[0];

		else//if heap isn't empty.
			return m_vec[1];
	}
    std::pair<TKey, TValue> Get(TValue target)
	{
		for (int i = 0; i<m_vec.size(); i++)//repeat this process until find the same vertex key
		{
			if (target == m_vec[i].second)//if find.
				return m_vec[i];//return the pair.
		}
		return m_vec[0];//There's no same vertex key.
	}

    bool IsEmpty()
	{
		if (m_vec.size() == 1)//if empty.
			return 1;
		else//not empty.
			return 0;
	}

    void DecKey(TValue target, TKey newKey)
	{
		int i;
		for (i = 0; i < m_vec.size(); i++)//Repeat this process until find the same vertex key
		{
			if (target == m_vec[i].second)//Find case.
			{
				m_vec[i].first = newKey;//change the weight
				break;
			}
		}
		pair<TKey, TValue> backup = m_vec[i];//data for sorting.
		int current = i;//start index.

		while (current != 1 && m_vec[current / 2].first > newKey)//if current==1 -> heap's root. // m_vec[current/2] -> parent index's value of current.
		{
			//case: parent's weight is bigger than backup's weight -> change the index.
			m_vec[current].first = m_vec[current / 2].first;
			m_vec[current].second = m_vec[current / 2].second;
			current /= 2;//move to parent
		}
		//if current==1 or newKey is bigger than the parent index's weight.
		//push.
		m_vec[current].first = backup.first;
		m_vec[current].second = backup.second;

		return;
	}

private:
    void Heapify(int index)
	{
		if (index == 0)//Push
		{
			int currentNode = m_vec.size() - 1;//start index
			pair<TKey, TValue> backup = m_vec.back();//the value for compare.

			while (currentNode != 1 && m_vec[currentNode / 2].first > backup.first)//if current==1 -> heap's root. // m_vec[current/2] -> parent index's value of current.
			{
				//case: parent's weight is bigger than backup's weight -> change the index.
				m_vec[currentNode].first = m_vec[currentNode / 2].first;
				m_vec[currentNode].second = m_vec[currentNode / 2].second;
				currentNode /= 2;//move to parent
			}
			//if current==1 or newKey is bigger than the parent index's weight.
			//push.
			m_vec[currentNode].first = backup.first;
			m_vec[currentNode].second = backup.second;
		}
		else if (index == 1)//Pop
		{
			if (IsEmpty())//empty case.
				return;

			pair<TKey, TValue> lastE = m_vec.back();//the value for compare.

			int currentNode = 1;//compare is started from root.
			int child = 2;//child of currentNode

			while (child <= m_vec.size() - 1)//repeat this process until currentNode isn't leaf node.
			{
				if (child < m_vec.size() - 1 && m_vec[child].first > m_vec[child + 1].first)// case: currentNode has two child and right child's weight is bigger than left's child.
					child++;//child <= right child.

				if (lastE.first <= m_vec[child].first)//compare last index's weight with child's weight.
					break;//case: last index's weight is smaller than child index's weight.

						  //else change currentNode index's value and child index's value.
				m_vec[currentNode].first = m_vec[child].first;
				m_vec[currentNode].second = m_vec[child].second;
				currentNode = child; child *= 2;//move currntNode to child. // move child to child's left child.
			}
			//case: currentNode is leaf or lastE's weight is smaller than child index's weight.
			m_vec[currentNode].first = lastE.first;
			m_vec[currentNode].second = lastE.second;

			m_vec.resize(m_vec.size() - 1);//reduce the size.
		}
	}
};

#endif
