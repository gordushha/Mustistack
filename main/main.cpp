#include "Stack.h"
#include "MultiStack.h"

int main() 
{
	cout << "Testing reallocation" << endl;

	TMultiStack<int> a(10, 3);
	a.Push(5, 0);
	a.Push(3, 0);
	a.Push(17, 0);
	a.Push(4, 0);
	a.Push(10, 1);
	a.Push(6, 2);
	a.Push(2, 1);
	
	cout << a;
	
	return 0;
}
