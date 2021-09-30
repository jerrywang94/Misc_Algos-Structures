#pragma once
#include <random>
#include <vector>

  /// <summary>
	/// Performs Fisher-yates shuffle on elements contained within Container T.
	/// Uses std::chrono::system_clock to seed a mt19937 engine to generate
	/// random vals for swapping.
	/// 
	/// Optionally allows for shuffling # elements < size of the container. If this is selected
	/// the first shuffle_number elements of container will be randomly chosen
	/// elements. Remaining elements are not guaranteed to have random positions relative
	/// to initial positions.
	/// </summary>
	/// <typeparam name="T">Must have iterator .begin() and .end() defined.
	/// Must be MoveAssignable and MoveConstructible.</typeparam>
	/// <param name="arr">Container to be shuffled</param>
	/// <param name="shuffle_number"># of elements to shuffle. Defaults to 0; returns if less than 0;
	/// automatically set to size of arr if greater than size of arr</param>
	template <class T>
	void fy_shuffle(T& arr, int shuffle_number = 0) { // implementation of fisher yates shuffle

		if (shuffle_number < 0)
			return;

		int arr_size = static_cast<int> (arr.end() - arr.begin());

		if (shuffle_number == 0 || shuffle_number >= arr_size)
			shuffle_number = arr_size - 1;

		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::mt19937 mgen(seed);

		for (int i = 0; i < shuffle_number; i++) {
			// re-construct udist each time to change bounds - apparently lightweight operation
			std::uniform_int_distribution<int> udist(i, (arr_size - 1));
			std::swap(arr[i], arr[udist(mgen)]);
		}

		return;

	}
