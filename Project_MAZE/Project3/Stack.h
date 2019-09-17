#ifndef STACK_H
#define STACK_H

#ifndef NULL
#define NULL    0
#endif

template <typename T>
class Stack
{
private:
    class StackNode
    {
    public:
        T Data;
        StackNode* pNext;

        StackNode(T data) : Data(data), pNext(NULL) {}
    };

private:
    // the head pointer of the stack
    StackNode* m_pTop;

public:
	Stack() { m_pTop = NULL; }
    ~Stack()
	{
		StackNode * pCur;
		while (m_pTop != NULL)//repeat this process until StackNode isn't exist
		{
			pCur = m_pTop;
			m_pTop = m_pTop->pNext;
			delete(pCur);//delete stackNode using pCur.
		}
	}

    void Push(T data)
	{
		StackNode * newNode = new StackNode(data);//make new stacknode
		newNode->pNext = m_pTop;//connect newNode to Stack's head.
		m_pTop = newNode;//move head
	}

    void Pop()
	{
		if (IsEmpty())//there's no node in stack
		{
			return;
		}
		StackNode * rNode = m_pTop;//Store head's address
		m_pTop = m_pTop->pNext;//Move head to next node.
		delete(rNode);//delete rNode.
	}
    T Top()
	{
		if (IsEmpty())//there's no Node in stack
			return NULL;

		return m_pTop->Data;//return last-in data.
	}

    bool IsEmpty()
	{
		if (m_pTop == NULL)//empty
			return 1;//return TRUE
		else
			return 0;//not empty -> return FALSE
	}
};

#endif
