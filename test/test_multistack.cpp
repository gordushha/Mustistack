#include <../gtest/gtest.h>
#include "MultiStack.h"

TEST(multistack, default_constructor_works)
{
	ASSERT_NO_THROW(TMultiStack<int> stk);
}

TEST(multistack, init_constructor_works)
{
	ASSERT_NO_THROW(TMultiStack<int> stk(1, 1));
}

TEST(multistack, cant_create_with_wrong_index)
{
	ASSERT_ANY_THROW(TMultiStack<int> stk(-1, -1));
}


TEST(multistack, copy_constructor_works)
{
	TMultiStack<int> stk(5, 3);

	stk.Push(3, 0);
	stk.Push(5, 1);
	stk.Push(7, 2);

	TMultiStack<int> stk2(stk);

	EXPECT_EQ(3, stk2.Pop(0));
}

TEST(multistack, can_push_and_pop)
{
	TMultiStack<int> stk(5, 3);

	stk.Push(3, 0);
	stk.Push(5, 1);
	stk.Push(7, 2);

	EXPECT_EQ(3, stk.Pop(0));
	EXPECT_EQ(5, stk.Pop(1));
	EXPECT_EQ(7, stk.Pop(2));
}

TEST(multistack, reallocation_works)
{
	TMultiStack<int> stk(10, 3);

	stk.Push(11, 0);
	stk.Push(12, 0);

	stk.Push(21, 1);
	stk.Push(22, 1);

	stk.Push(31, 2);
	stk.Push(32, 2);

	stk.Push(13, 0);
	stk.Push(14, 0);

	EXPECT_EQ(14, stk.Pop(0));
	EXPECT_EQ(13, stk.Pop(0));
	EXPECT_EQ(12, stk.Pop(0));
	EXPECT_EQ(11, stk.Pop(0));
}

TEST(multistack, cant_pop_empty)
{
	TMultiStack<int> B(5, 3);

	ASSERT_ANY_THROW(B.Pop(0));
}

TEST(multistack, cant_push_full)
{
	TMultiStack<int> B(3, 1);

	B.Push(1, 0);
	B.Push(2, 0);
	B.Push(3, 0);

	ASSERT_ANY_THROW(B.Push(0, 0));
}

