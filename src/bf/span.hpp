#ifndef SPAN_HPP
#define SPAN_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>

template<class It>
class span
{
	It _begin, _end;

public:
	span() = default;

	template<class T>
	span(const T& container) :
		_begin{container.begin()},
		_end{container.end()}
	{}

	template<class T>
	span(T& container) :
		span{static_cast<const T&>(container)}
	{}

	span(It begin, It end) :
		_begin{begin},
		_end{end}
	{}

	span(It begin, size_t count) :
		_begin{begin},
		_end{begin + static_cast<intptr_t>(count)}
	{}

	template<class T>
	T as_container() const
	{
		return {_begin, _end};
	}

	auto& operator[](size_t off)
	{
		return _begin[off];
	}

	const auto& operator[](size_t off) const
	{
		return _begin[off];
	}

	It begin() const
	{
		return _begin;
	}

	It end() const
	{
		return _end;
	}

	auto rbegin() const
	{
		return std::reverse_iterator{_end};
	}

	auto rend() const
	{
		return std::reverse_iterator{_begin};
	}

	size_t size() const
	{
		return static_cast<size_t>(std::distance(_begin, _end));
	}

	bool empty() const
	{
		return _begin == _end;
	}

	template<class T>
	bool operator==(span<T> other) const
	{
		return std::equal(begin(), end(), other.begin(), other.end());
	}

	template<class T>
	bool operator!=(span<T> other) const
	{
		return !operator==(other);
	}
};

template<typename T> span(const T&) -> span<typename T::const_iterator>;
template<typename T> span(T&) -> span<typename T::iterator>;

#endif
