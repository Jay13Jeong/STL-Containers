#ifndef UTILS_CLASS_HPP
# define UTILS_CLASS_HPP

# include <iostream>
# include <string>
# include <memory>
# include <limits>
# include <cstddef>
# include <sstream>
# include "iterator_traits.hpp"

namespace ft {

//첫번째 탬플릿인자가 false가 되면 sfinae를 유도.
template <bool, typename T = void>
struct enable_if;

//첫번째 탬플릿인자가 true가 되면 두번째 인자를 type으로 선언.
template <typename T>
struct enable_if<true, T> {
	typedef T type;
};

//int계열 부울 탬플릿 특수화.
template <typename T>
struct is_integral
{
	static bool const value = false;
};

template <>
struct is_integral<bool> 
{
	static bool const value = true;
};

template <>
struct is_integral<char> 
{
	static bool const value = true;
};

template <>
struct is_integral<wchar_t> 
{
	static bool const value = true;
};

template <>
struct is_integral<short> 
{
	static bool const value = true;
};

template <>
struct is_integral<int> 
{
	static bool const value = true;
};

template <>
struct is_integral<long> 
{
	static bool const value = true;
};

//두 반복자의의 요소를 비교하는 템플릿 함수. C1부분만 모두 동일하면 true반환
template <class C1, class C2>
bool	equal(C1 begin1, C1 end1, C2 begin2)
{
	while (begin1 != end1)
		if (*(begin1++) != *(begin2++))
			return false;
	return true;
}

//두 반복자가 완벽히 동일하거나 1번 반복자가 사전순으로 빠르면 true.
//아니면 false를 반환하는 템플릿 함수. 
template <class C1, class C2>
bool	lexicographical_compare(C1 begin1, C1 end1, C2 begin2, C2 end2)
{
	//null을 만나거나 두 데이터가 다를 때까지 루프. 
	while (begin1 != end1 && begin2 != end2 && *begin1 == *begin2)
	{
		++begin1;
		++begin2;
	}
	if (begin1 == end1) //첫 번째 반복자를 모두 비교했을 때.
		return (begin2 != end2); //두 반복자의 요소가 동일하면 true 아니면 false.
	else if (begin2 == end2) //또는 두번째 반복자를 모두 비교했을 때.
		return (false); //1번 반복자가 더 길어서 false. 
	return (*begin1 < *begin2); //두 반복자 요소가 다르면 사전순으로 비교.
}

//키,값을 묶어서 정의하는 템플릿 구조체.
template <class T1, class T2>
struct	pair
{
	typedef T1		first_type;
	typedef T2		second_type;

	first_type		first;
	second_type		second;

	pair(void) : first(), second() { };
	template<class U, class V>
	pair(const pair<U, V> &src) : first(src.first), second(src.second) { };
	pair(const first_type &a, const second_type &b) : first(a), second(b) { };

	pair	&operator=(const pair &src) {
		if (this == &src)
			return (*this);
		this->first = src.first;
		this->second = src.second;
		return (*this);
	}
};


template <class T1, class T2>
pair<T1, T2>
make_pair(const T1& x, const T2& y)
{ return pair<T1, T2>(x, y); }

template <class T1, class T2>
bool operator==(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
	return (lhs.first == rhs.first && lhs.second == rhs.second);
}

template <class T1, class T2>
bool operator!=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
	return !(lhs == rhs);
}

template <class T1, class T2>
bool operator< (const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
	return lhs.first < rhs.first || (!(rhs.first < lhs.first) && lhs.second < rhs.second);
}

template <class T1, class T2>
bool operator<=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
	return !(rhs < lhs);
}

template <class T1, class T2>
bool operator> (const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
	return (rhs < lhs);
}

template <class T1, class T2>
bool operator>=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs) {
	return !(lhs < rhs);
}

}
#endif
