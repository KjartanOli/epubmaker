#ifndef FITER_H
	#define FILTER_H
	#include <vector>
	#include <functional>

	template <typename T>
	std::vector<T> filter(const std::vector<T>& vec, std::function<bool(T)> condition)
	{
		std::vector<T> res;

		for (const T& el : vec)
		{
			if (condition(el))
			{
				res.push_back(el);
			}
		}

		return res;
	}
#endif
