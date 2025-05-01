#include <gtest/gtest.h>
#include "Emu/RSX/Common/simple_array.hpp"

namespace rsx
{
	class simple_array_test : public simple_array<u32>
	{
	public:
		bool data_is_local_storage() { return is_local_storage(); }
	};

	TEST(SimpleArray, test_empty)
	{
		simple_array<u32> arr;
		EXPECT_NE(nullptr, arr.data());
		EXPECT_EQ(arr.data(), arr.begin());
		EXPECT_EQ(arr.begin(), arr.end());
		EXPECT_EQ(true, arr.empty());
		EXPECT_EQ(0, arr.size());
		EXPECT_EQ(0, arr.size_bytes());
		EXPECT_EQ(16, arr.capacity());

		arr.clear();
		EXPECT_EQ(0, arr.size());

		arr.reserve(0u);
		EXPECT_EQ(0, arr.size());
		EXPECT_EQ(16, arr.capacity());

		arr.resize(0u);
		EXPECT_EQ(0, arr.size());
		EXPECT_EQ(16, arr.capacity());

		simple_array<u32> arr2(0);
		EXPECT_NE(nullptr, arr2.data());
		EXPECT_EQ(arr2.data(), arr2.begin());
		EXPECT_EQ(arr2.begin(), arr2.end());
		EXPECT_EQ(true, arr2.empty());
		EXPECT_EQ(0, arr2.size());
		EXPECT_EQ(0, arr2.size_bytes());
		EXPECT_EQ(16, arr2.capacity());

		arr2.clear();
		EXPECT_EQ(0, arr2.size());

		arr2.reserve(0u);
		EXPECT_EQ(0, arr2.size());
		EXPECT_EQ(16, arr2.capacity());

		arr2.resize(0u);
		EXPECT_EQ(0, arr2.size());
		EXPECT_EQ(16, arr2.capacity());
	}

	TEST(SimpleArray, test_max_stack)
	{
		simple_array_test arr(16);

		EXPECT_NE(nullptr, arr.data());
		EXPECT_EQ(true, arr.data_is_local_storage());
		EXPECT_EQ(arr.data(), arr.begin());
		EXPECT_EQ(arr.begin() + 16, arr.end());
		EXPECT_EQ(false, arr.empty());
		EXPECT_EQ(16, arr.size());
		EXPECT_EQ(16 * sizeof(u32), arr.size_bytes());
		EXPECT_EQ(16, arr.capacity());

		arr.reserve(16u);
		EXPECT_EQ(16, arr.size());
		EXPECT_EQ(16, arr.capacity());
		EXPECT_EQ(true, arr.data_is_local_storage());

		arr.resize(16u);
		EXPECT_EQ(16, arr.size());
		EXPECT_EQ(16, arr.capacity());
		EXPECT_EQ(true, arr.data_is_local_storage());

		arr.clear();
		EXPECT_EQ(0, arr.size());
		EXPECT_EQ(true, arr.data_is_local_storage());

		arr.reserve(0u);
		EXPECT_EQ(0, arr.size());
		EXPECT_EQ(16, arr.capacity());
		EXPECT_EQ(true, arr.data_is_local_storage());

		arr.resize(0u);
		EXPECT_EQ(0, arr.size());
		EXPECT_EQ(16, arr.capacity());
		EXPECT_EQ(true, arr.data_is_local_storage());

		arr.reserve(16u);
		EXPECT_EQ(0, arr.size());
		EXPECT_EQ(16, arr.capacity());
		EXPECT_EQ(true, arr.data_is_local_storage());

		arr.resize(16u);
		EXPECT_EQ(16, arr.size());
		EXPECT_EQ(16, arr.capacity());
		EXPECT_EQ(true, arr.data_is_local_storage());
	}

	TEST(SimpleArray, test_heap)
	{
		simple_array_test arr(17);

		EXPECT_NE(nullptr, arr.data());
		EXPECT_EQ(false, arr.data_is_local_storage());
		EXPECT_EQ(arr.data(), arr.begin());
		EXPECT_EQ(arr.begin() + 17, arr.end());
		EXPECT_EQ(false, arr.empty());
		EXPECT_EQ(17, arr.size());
		EXPECT_EQ(17 * sizeof(u32), arr.size_bytes());
		EXPECT_EQ(17, arr.capacity());

		arr.reserve(17u);
		EXPECT_EQ(17, arr.size());
		EXPECT_EQ(17, arr.capacity());
		EXPECT_EQ(false, arr.data_is_local_storage());

		arr.resize(17u);
		EXPECT_EQ(17, arr.size());
		EXPECT_EQ(17, arr.capacity());
		EXPECT_EQ(false, arr.data_is_local_storage());

		arr.clear();
		EXPECT_EQ(0, arr.size());
		EXPECT_EQ(false, arr.data_is_local_storage());

		arr.reserve(0u);
		EXPECT_EQ(0, arr.size());
		EXPECT_EQ(17, arr.capacity());
		EXPECT_EQ(false, arr.data_is_local_storage());

		arr.resize(0u);
		EXPECT_EQ(0, arr.size());
		EXPECT_EQ(17, arr.capacity());
		EXPECT_EQ(false, arr.data_is_local_storage());

		arr.reserve(17u);
		EXPECT_EQ(0, arr.size());
		EXPECT_EQ(17, arr.capacity());
		EXPECT_EQ(false, arr.data_is_local_storage());

		arr.resize(17u);
		EXPECT_EQ(17, arr.size());
		EXPECT_EQ(17, arr.capacity());
		EXPECT_EQ(false, arr.data_is_local_storage());
	}

	/*TEST(SimpleArray, test_reserve)
	{
		simple_array<u32> arr;
		EXPECT_EQ(0, arr.reserve([](u32){ return true; }));
	}

	TEST(SimpleArray, test_resize)
	{
		simple_array<u32> arr;
		EXPECT_EQ(0, arr.resize([](u32){ return true; }));
	}

	TEST(SimpleArray, test_reduce)
	{
		simple_array<u32> arr;
		EXPECT_EQ(0, arr.reduce([](u32){ return true; }));
	}

	TEST(SimpleArray, test_map)
	{
		simple_array<u32> arr;
		EXPECT_EQ(0, arr.map([](u32){ return true; }));
	}

	TEST(SimpleArray, test_sort)
	{
		simple_array<u32> arr;
		EXPECT_EQ(0, arr.sort([](u32){ return true; }));
	}

	TEST(SimpleArray, test_swap)
	{
		simple_array<u32> arr;
		EXPECT_EQ(0, arr.swap([](u32){ return true; }));
	}

	TEST(SimpleArray, test_any)
	{
		simple_array<u32> arr;
		EXPECT_EQ(0, arr.any([](u32){ return true; }));
	}

	TEST(SimpleArray, test_emplace_back)
	{
		simple_array<u32> arr;
		EXPECT_EQ(0, arr.emplace_back([](u32){ return true; }));
	}

	TEST(SimpleArray, test_push_back)
	{
		simple_array<u32> arr;
		EXPECT_EQ(0, arr.push_back([](u32){ return true; }));
	}

	TEST(SimpleArray, test_pop_back)
	{
		simple_array<u32> arr;
		EXPECT_EQ(0, arr.pop_back([](u32){ return true; }));
	}

	TEST(SimpleArray, test_insert)
	{
		simple_array<u32> arr;
		EXPECT_EQ(0, arr.insert([](u32){ return true; }));
	}

	TEST(SimpleArray, test_erase_if)
	{
		simple_array<u32> arr;
		EXPECT_EQ(0, arr.erase_if([](u32){ return true; }));
	}

	TEST(SimpleArray, test_access)
	{
		EXPECT_EQ(nullptr, arr.front());
		EXPECT_EQ(nullptr, arr.back());
		EXPECT_EQ(nullptr, arr[]());
	}*/
}
