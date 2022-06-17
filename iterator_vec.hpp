#ifndef ITERATOR_VEC_CLASS_HPP
# define ITERATOR_VEC_CLASS_HPP

# include "utils.hpp"

namespace ft 
{
template <typename _Iter>
class iter_vec 
{
public:
	typedef _Iter															iterator_type;
	typedef typename ft::iterator_traits<iterator_type>::value_type			value_type;
	typedef typename ft::iterator_traits<iterator_type>::difference_type	difference_type;
	typedef typename ft::iterator_traits<iterator_type>::pointer			pointer;
    typedef typename ft::iterator_traits<iterator_type>::reference			reference;
    typedef typename ft::iterator_traits<iterator_type>::iterator_category	iterator_category;

private:
	iterator_type _value;

public:
	iter_vec(void) : _value(NULL) {}
	iter_vec(const iter_vec  &src) : _value(src._value) {}
	iter_vec(_Iter p) : _value(p) {}

    template <class UP>
    iter_vec(const iter_vec<UP> &src): _value(src.base())
    { }

	virtual ~iter_vec() {};

    reference operator*() const
    {
        return *_value;
    }
	
    pointer  operator->() const
    {
        return _value;
    }

	//전위
    iter_vec& operator++()
    {
        ++_value;
        return *this;
    }

	//후위
    iter_vec  operator++(int)
    {iter_vec tmp(*this); ++(*this); return tmp;}

    iter_vec& operator--()
    {
        --_value;
        return *this;
    }
	
    iter_vec  operator--(int)
    {iter_vec tmp(*this); --(*this); return tmp;}
    
	iter_vec  operator+ (difference_type d_) const
    {iter_vec tmp(*this); tmp += d_; return tmp;}
    
	iter_vec& operator+=(difference_type d_)
    {
        _value += d_;
        return *this;
    }

    iter_vec  operator- (difference_type d_) const
    {return *this + (-d_);}

    iter_vec& operator-=(difference_type d_)
    {*this += -d_; return *this;}

    reference    operator[](difference_type d_) const
    {
        return _value[d_];
    }

    iterator_type base() const
	{return _value;}

};

template <class _Iter1>
bool
operator==(const iter_vec<_Iter1>& a, const iter_vec<_Iter1>& b) 
{
    return a.base() == b.base();
}

template <class _Iter1, class _Iter2>
bool
operator==(const iter_vec<_Iter1>& a, const iter_vec<_Iter2>& b) 
{
    return a.base() == b.base();
}

template <class _Iter1>
bool
operator<(const iter_vec<_Iter1>& a, const iter_vec<_Iter1>& b) 
{
    return a.base() < b.base();
}

template <class _Iter1, class _Iter2>
bool
operator<(const iter_vec<_Iter1>& a, const iter_vec<_Iter2>& b) 
{
    return a.base() < b.base();
}

template <class _Iter1>
bool
operator!=(const iter_vec<_Iter1>& a, const iter_vec<_Iter1>& b) 
{
    return !(a == b);
}

template <class _Iter1, class _Iter2>
bool
operator!=(const iter_vec<_Iter1>& a, const iter_vec<_Iter2>& b) 
{
    return !(a == b);
}

template <class _Iter1>
bool
operator>(const iter_vec<_Iter1>& a, const iter_vec<_Iter1>& b) 
{
    return b < a;
}

template <class _Iter1, class _Iter2>
bool
operator>(const iter_vec<_Iter1>& a, const iter_vec<_Iter2>& b) 
{
    return b < a;
}

template <class _Iter1>
bool
operator>=(const iter_vec<_Iter1>& a, const iter_vec<_Iter1>& b) 
{
    return !(a < b);
}

template <class _Iter1, class _Iter2>
bool
operator>=(const iter_vec<_Iter1>& a, const iter_vec<_Iter2>& b) 
{
    return !(a < b);
}

template <class _Iter1>
bool
operator<=(const iter_vec<_Iter1>& a, const iter_vec<_Iter1>& b) 
{
    return !(b < a);
}

template <class _Iter1, class _Iter2>
bool
operator<=(const iter_vec<_Iter1>& a, const iter_vec<_Iter2>& b) 
{
    return !(b < a);
}

template <class _Iter1, class _Iter2>
typename iter_vec<_Iter1>::difference_type
operator-(const iter_vec<_Iter1>& a, const iter_vec<_Iter2>& b)
{
    return a.base() - b.base();
}

template <class _Iter1>
iter_vec<_Iter1>
operator+(typename iter_vec<_Iter1>::difference_type d, iter_vec<_Iter1> a) 
{
    a += d;
    return a;
}

}
#endif
