#pragma once

#include <vector>
#include <memory>

template <class T>
class PriorityHeap
{
private:
	template <class T>
	class HeapElement
	{
	public:
		T data;
		float prio;

		HeapElement(T data, float prio) : data(data), prio(prio) {};
	};

	std::vector<std::shared_ptr<HeapElement<T>>> heap;
	int heapSize;

	void swap(int i1, int i2);
	int minChild(int index);

public:
	PriorityHeap() { heap.push_back(nullptr); heapSize = 0; };

	void insert(T data, float prio);
	T getMin();
	int getSize() { return heapSize; };
};

template<class T>
void PriorityHeap<T>::swap(int i1, int i2)
{
	auto pom = heap[i1];
	heap[i1] = heap[i2];
	heap[i2] = pom;
}

template<class T>
void PriorityHeap<T>::insert(T data, float prio)
{
	auto newElem = std::make_shared<HeapElement<T>>(data, prio);

	heap.push_back(newElem);
	heapSize++;

	int nIndex = heapSize;
	while (nIndex != 1)
	{
		int parentIndex = (int)(nIndex / 2); // int cast not floor as we are in +values
		auto parentElem = heap[parentIndex];

		if (parentElem->prio >= heap[nIndex]->prio)
		{
			swap(parentIndex, nIndex);
			nIndex = parentIndex;
		}
		else
			break;
	}
	int pom = 10;
}

template<class T>
T PriorityHeap<T>::getMin()
{
	swap(1, heapSize);
	auto minData = heap[heapSize]->data;

	heap.erase(heap.begin()+heapSize);
	heapSize--;

	int index = 1;
	while (true)
	{
		int minSide = minChild(index);
		if (minSide == -1)
			break;

		int iChild;
		if (minSide == 0)
		{
			iChild = 2 * index;
			swap(index, iChild);
		}
		else
		{
			iChild = 2 * index + 1;
			swap(index, iChild);
		}
		index = iChild;
	}

	return minData;
}

template<class T>
int PriorityHeap<T>::minChild(int index)
{
	int iChild1 = 2 * index;
	int iChild2 = 2 * index + 1;
	if (heapSize < iChild1) // no children
	{
		return -1;
	}
	if (heapSize == iChild1) // one child
	{
		if (heap[iChild1]->prio < heap[index]->prio)
		{
			return 0;
		}
		return -1;
	}
	if (heap[index]->prio > heap[iChild1]->prio || heap[index]->prio > heap[iChild2]->prio) // we will swap something
	{
		if (heap[iChild1]->prio  < heap[iChild2]->prio)
			return 0;
		else
			return 1;
	}
	else
		return -1;
}
