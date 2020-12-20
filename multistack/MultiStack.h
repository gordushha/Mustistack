#pragma once
#include "Stack.h"

template <class T>
class TMultiStack
{
protected:
	int StackCount;
	int length;

	T* data;
	T** oldData;
	TStack<T>* stacks;

	void StackRelocation(int index);
	int free_space()
	{
		int freeSize = 0;
		for (int i = 0; i < StackCount; i++)
			freeSize += stacks[i].GetSize() - stacks[i].GetCount();
		return freeSize;
	}
	void move_left(int i, T** newData, int* sizes)
	{
		for (int j = 0; j < stacks[i].GetCount(); j++)
			newData[i][j] = oldData[i][j];
		stacks[i].SetData(newData[i], sizes[i], stacks[i].GetCount());
	}
	void move_right(int i, int k, T** newData, int* sizes)
	{
		for (k = i; k < StackCount; k++)
			if (newData[k] > oldData[k])
				continue;
			else
				break;
		k--;

		for (int s = k; s >= i; s--)
		{
			for (int j = stacks[s].GetCount() - 1; j >= 0; j--)
				newData[s][j] = oldData[s][j];

			stacks[s].SetData(newData[s], sizes[s], stacks[s].GetCount());
		}
	}

	int* memalloc()
	{
		int to_add = floor((double)length / StackCount);

		int* sizes = new int[StackCount];
		for (int i = 0; i < StackCount - 1; i++)
			sizes[i] = to_add;

		sizes[StackCount - 1] = length - to_add * (StackCount - 1);
		return sizes;
	}

	void move(int* sizes)
	{
		T** newData = new T * [StackCount];
		int k = 0;
		for (int i = 0; i < StackCount; i++)
		{
			newData[i] = &(data[k]);
			k += sizes[i];
		}

		for (int i = 0; i < StackCount; i++)
		{
			if (newData[i] == oldData[i])
				stacks[i].SetData(newData[i], sizes[i], stacks[i].GetCount());

			else if (newData[i] < oldData[i])
			{
				move_left(i, newData, sizes);
			}
			else if (newData[i] > oldData[i])
			{
				move_right(i, k, newData, sizes);
			}

		}

		T** buffer = oldData;
		oldData = newData;
		delete[] buffer;
		delete[] sizes;
	}

public:
	TMultiStack(int length = 1, int StackCount = 1);
	TMultiStack(TMultiStack<T>& _v);
	~TMultiStack();

	TMultiStack<T>& operator =(TMultiStack<T>& _v);

	void Push(T d, int i);
	T Pop(int i);

	void realloc_two();

	bool IsEmpty(int i) const;
	bool IsFull(int i) const;

	template<class T1>
	friend ostream& operator<< (ostream& ostr, const TMultiStack<T1>& A);
};

template<class T>
ostream& operator<<(ostream& ostr, const TMultiStack<T>& A)
{
	for (int i = 0; i < A.StackCount; i++)
		ostr << "[" << A.stacks[i] << "]" << endl;
	return ostr;
}

template<class T>
inline void TMultiStack<T>::StackRelocation(int index)
{
	int freeSize = free_space();
	
	if (freeSize == 0)
		throw logic_error("stack full");

	int to_add = floor((double)freeSize / StackCount);

	int* sizes = new int[StackCount];
	for (int i = 0; i < StackCount - 1; i++)
		sizes[i] = stacks[i].GetCount() + to_add;

	sizes[StackCount - 1] = stacks[StackCount - 1].GetCount() + freeSize - to_add * (StackCount - 1);

	move(sizes);
}

template<class T>
inline void TMultiStack<T>::realloc_two()
{
	int max = 0, min = 0;
	int temp1 = stacks[0].GetSize() - stacks[0].GetCount();
	int temp2 = stacks[0].GetSize() - stacks[0].GetCount();

	for (int i = 0; i < StackCount; i++)
	{
		if (stacks[i].GetSize() - stacks[i].GetCount() > temp1)
		{
			temp1 = stacks[i].GetSize() - stacks[i].GetCount();
			max = i;
		}
		if (stacks[i].GetSize() - stacks[i].GetCount() < temp2)
		{
			temp1 = stacks[i].GetSize() - stacks[i].GetCount();
			min = i;
		}
	}

	if (max == min)
		return;
	else
	{
		int pool = temp1 + temp2;
		temp1 = int(2.0 / 3 * pool);
		temp2 = pool - temp1;
		
		int* sizes = new int[StackCount];
		for (int i = 0; i < StackCount - 1; i++)
		{
			if (i == max)
				sizes[i] = temp1;
			else if (i == min)
				sizes[i] = temp2;
			else
				sizes[i] = stacks[i].GetSize();
		}

		move(sizes);
	}
}

template<class T>
TMultiStack<T>::TMultiStack(int length, int StackCount)
{
	if (length <= 0 || StackCount <= 0)
		throw logic_error("invalid length");

	this->StackCount = StackCount;
	this->length = length;

	data = new T[length];
	for (int i = 0; i < length; i++)
		data[i] = 0;

	int* sizes = memalloc();

	oldData = new T * [StackCount];

	stacks = new TStack<T>[StackCount];
	int k = 0;
	for (int i = 0; i < StackCount; i++)
	{
		stacks[i].SetData(&(data[k]), sizes[i], 0);
		oldData[i] = &(data[k]);
		k += sizes[i];
	}
}

template <class T>
TMultiStack<T>::TMultiStack(TMultiStack<T>& _v)
{
	length = _v.length;
	StackCount = _v.StackCount;

	data = new T[length];
	for (int i = 0; i < length; i++)
		data[i] = _v.data[i];

	stacks = new TStack<T>[StackCount];

	for (int i = 0; i < StackCount; i++)
		stacks[i] = _v.stacks[i];
}

template<class T>
TMultiStack<T>::~TMultiStack()
{
	if (data != 0)
	{
		delete[] data;
		delete[] stacks;
	}
	length = 0;
	StackCount = 0;
}

template<class T>
inline TMultiStack<T>& TMultiStack<T>::operator=(TMultiStack<T>& _v)
{
	if (this == &_v)
		return *this;

	length = _v.length;
	StackCount = _v.StackCount;

	delete[] data;
	data = new T[length];
	for (int i = 0; i < length; i++)
		data[i] = _v.data[i];

	delete[] stacks;
	stacks = new TStack<T>[StackCount];
	for (int i = 0; i < StackCount; i++)
		stacks[i] = _v.stacks[i];

	return *this;
}

template<class T>
void TMultiStack<T>::Push(T d, int i)
{
	if (i < 0 || i >= StackCount)
		throw logic_error("invalid index");

	if (stacks[i].IsFull())
		StackRelocation(i);

	stacks[i].Push(d);
}

template<class T>
T TMultiStack<T>::Pop(int i)
{
	if (i < 0 || i >= StackCount)
		throw logic_error("invalid index");

	return stacks[i].Pop();
}

template<class T>
inline bool TMultiStack<T>::IsEmpty(int i) const
{

	if ((i < 0) || (i >= StackCount))
		throw logic_error("invalid index");

	return stacks[i].IsEmpty();
}

template<class T>
inline bool TMultiStack<T>::IsFull(int i) const
{
	if ((i < 0) || (i >= StackCount))
		throw logic_error("invalid index");

	return stacks[i].IsFull();
}
