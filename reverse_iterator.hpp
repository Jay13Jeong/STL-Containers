#ifndef REVERSE_ITERATOR_CLASS_HPP
# define REVERSE_ITERATOR_CLASS_HPP

# include "utils.hpp"

namespace ft 
{
template <class _Iter>
class reverse_iterator 
{
protected:
	_Iter current;
public:
	typedef _Iter							iterator_type;
	typedef typename iterator_traits<_Iter>::difference_type		difference_type;
	typedef typename iterator_traits<_Iter>::reference			reference;
	typedef typename iterator_traits<_Iter>::pointer				pointer;
	typedef typename iterator_traits<_Iter>::iterator_category		iterator_category;
	typedef typename iterator_traits<_Iter>::value_type      value_type;

    reverse_iterator() : current() {}
    
    explicit reverse_iterator(_Iter a) : current(a) {}

    template <class _Up>
    reverse_iterator(const reverse_iterator<_Up>& __u)
	: current(__u.base())
    { }

    template <class _Up>
    reverse_iterator& operator=(const reverse_iterator<_Up>& __u)
	{
        current = __u.base();
        return *this;
    }

    _Iter base() const {return current;}
    
    reference operator*() const {_Iter __tmp = current; return *--__tmp;}
    
    pointer  operator->() const {return &(this->operator*());}

    reverse_iterator& operator++() {--current; return *this;}
    
    reverse_iterator  operator++(int) {reverse_iterator __tmp(*this); --current; return __tmp;}
    
    reverse_iterator& operator--() {++current; return *this;}
    
    reverse_iterator  operator--(int) {reverse_iterator __tmp(*this); ++current; return __tmp;}
    
    reverse_iterator  operator+ (difference_type d_) const {return reverse_iterator(current - d_);}
    
    reverse_iterator& operator+=(difference_type d_) {current -= d_; return *this;}
    
    reverse_iterator  operator- (difference_type d_) const {return reverse_iterator(current + d_);}
    
    reverse_iterator& operator-=(difference_type d_) {current += d_; return *this;}
    
    reference         operator[](difference_type d_) const {return *(*this + d_);}

};

template <class _Iter1, class _Iter2>
bool
operator==(const reverse_iterator<_Iter1>& a, const reverse_iterator<_Iter2>& b)
{
    return a.base() == b.base();
}

template <class _Iter1, class _Iter2>
bool
operator<(const reverse_iterator<_Iter1>& a, const reverse_iterator<_Iter2>& b)
{
    return a.base() > b.base();
}

template <class _Iter1, class _Iter2>
bool
operator!=(const reverse_iterator<_Iter1>& a, const reverse_iterator<_Iter2>& b)
{
    return a.base() != b.base();
}

template <class _Iter1, class _Iter2>
bool
operator>(const reverse_iterator<_Iter1>& a, const reverse_iterator<_Iter2>& b)
{
    return a.base() < b.base();
}

template <class _Iter1, class _Iter2>
bool
operator>=(const reverse_iterator<_Iter1>& a, const reverse_iterator<_Iter2>& b)
{
    return a.base() <= b.base();
}

template <class _Iter1, class _Iter2>
bool
operator<=(const reverse_iterator<_Iter1>& a, const reverse_iterator<_Iter2>& b)
{
    return a.base() >= b.base();
}

template <class _Iter1, class _Iter2>
typename reverse_iterator<_Iter1>::difference_type
operator-(const reverse_iterator<_Iter1>& a, const reverse_iterator<_Iter2>& b)
{
    return b.base() - a.base();
}

template <class _Iter>
reverse_iterator<_Iter>
operator+(typename reverse_iterator<_Iter>::difference_type d_, const reverse_iterator<_Iter>& a)
{
    return reverse_iterator<_Iter>(a.base() - d_);
}

}
#endif
