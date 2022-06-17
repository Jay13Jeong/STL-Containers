#ifndef VECTOR_CLASS_HPP
# define VECTOR_CLASS_HPP

# include "iterator_vec.hpp"
# include "reverse_iterator.hpp"

namespace ft
{
template< typename T, typename Alloc = std::allocator<T> >
class vector
{
public:
	typedef T											value_type;
	typedef Alloc										allocator_type;
	typedef typename allocator_type::reference			reference;
	typedef typename allocator_type::const_reference	const_reference;
	typedef typename allocator_type::pointer			pointer;
	typedef typename allocator_type::const_pointer		const_pointer;
	typedef ptrdiff_t									difference_type;
	typedef size_t										size_type;
	typedef ft::iter_vec<pointer>						iterator;
    typedef ft::iter_vec<const_pointer>              	const_iterator;
	typedef ft::reverse_iterator<iterator>				reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>		const_reverse_iterator;

private:
	size_type 					_size;
	size_type 					_max_size;
	size_type 					_capacity;
	pointer						_begin;
	allocator_type				_alloc;

public:
	vector()
	: _size(0), _capacity(0), _begin(NULL), _alloc(Alloc())
	{
		_max_size = _alloc.max_size();
		if (typeid(value_type).name()[0] == 'c')
			_max_size /= 2;
	}

	vector(const Alloc& alloc)
	: _size(0), _capacity(0), _begin(NULL), _alloc(alloc)
	{
		_max_size = _alloc.max_size();
		if (typeid(value_type).name()[0] == 'c')
			_max_size /= 2;
	}

	vector(size_type count, const T& value = T(), const Alloc& alloc = Alloc())
	: _size(count), _capacity(0), _begin(NULL), _alloc(alloc)
	{
		_max_size = _alloc.max_size();
		if (typeid(value_type).name()[0] == 'c')
			_max_size /= 2;
		if (_size > 0)
		{
			_capacity = _size;
			_begin = _alloc.allocate(_capacity);
			for (size_type i = 0; i < _size; i++)
				_alloc.construct(_begin + i, value);
		}
	}

	template<class Iter_type>
	vector(Iter_type first, Iter_type last,
			const Alloc& alloc = Alloc(),
			typename ft::enable_if<!ft::is_integral<Iter_type>::value>::type* = 0)
	: _capacity(0), _begin(NULL), _alloc(alloc)
	{
		_max_size = _alloc.max_size();
		if (typeid(value_type).name()[0] == 'c')
			_max_size /= 2;
		this->_size = __iter_offset(first, last);
		if (this->_size > 0)
		{
			_capacity = _size;
			this->_begin = this->_alloc.allocate(_capacity);
			Iter_type it = first;
			size_type i = 0;
			for (; it != last; it++)
			{
				_alloc.construct(_begin + i, *it);
				i++;
			}
			_capacity = _size;
		}
	}

	vector(const vector& other)
	: _size(other._size), _capacity(other._capacity), _begin(NULL),
		_alloc(other._alloc)
	{
		_max_size = _alloc.max_size();
		if (typeid(value_type).name()[0] == 'c')
			_max_size /= 2;

		if (_size > 0)
		{
			_begin = _alloc.allocate(_capacity);
			for (size_type i = 0; i < _size; i++)
				_alloc.construct(_begin + i, other._begin[i]);
		}
	}

	vector &
	operator=(const vector& x)
	{ 
		size_type new_capacity;

		if (x._size > this->_capacity)
			new_capacity = x._size;
		else
			new_capacity = this->_capacity;
		pointer new_begin = _alloc.allocate(new_capacity);	
		for (size_type i = 0; i < x._size; i++)
			_alloc.construct(new_begin + i, x._begin[i]);
		for (size_type i = 0; i < this->_size; i++)
			_alloc.destroy(_begin + i);
		_alloc.deallocate(_begin, this->_capacity);
		_begin = new_begin;
		this->_capacity = new_capacity;
		this->_size = x._size;
	
		return *this;
	}

	virtual ~vector()
	{
		for (size_type i = 0; i < _size; i++)
			_alloc.destroy(_begin + i);
		if (_capacity != 0)
			_alloc.deallocate(_begin, _capacity);
	}

	size_type
	size() const 
	{ return _size; }

	size_type
	max_size() const
	{ return _max_size; }

	void
	resize(size_type n, value_type val = value_type())
	{
		//1. make capacity env.
		if (n > _capacity)
		{
			this->__realloc_begin(__custom_cap(n));
		}
		//2. fill extra space (dummy value).
		if (n > _size)
		{
			for (size_type i = _size; i < n; i++)
				_alloc.construct(_begin + i, val);
		}
		//3. delete extra elements.
		if (n < _size)
		{
			for (size_type i = n; i < _size; i++)
				_alloc.destroy(_begin + i);
		}
		_size = n;
	}

	size_type
	capacity() const
	{ return this->_capacity; }

	bool
	empty() const
	{ return (this->_size == 0); }

	void
	reserve(size_type n)
	{
		if (n > _max_size)
			throw std::length_error("vector");
		if (n <= _capacity)
			return ;
		this->__realloc_begin(n);
		_capacity = n;
	}

	template <class Iter_type>
	void
	assign(Iter_type first, Iter_type last,
			typename ft::enable_if<!ft::is_integral<Iter_type>::value>::type* = 0)
	{
		size_type offset_size = __iter_offset(first, last);
		if (offset_size > _capacity)
		{
			size_type new_capacity = offset_size; 
			this->__destroy_begin();
			_begin = _alloc.allocate(new_capacity);
			size_type i = 0;
			for (Iter_type it = first; it != last; it++)
				_alloc.construct(_begin + i++, *it);
			_capacity = new_capacity;
		}
		else
		{	
			for (size_type i = 0; i < _size; i++)
				_alloc.destroy(_begin + i);
			size_type i = 0;
			for (Iter_type it = first; it != last; it++)
				_alloc.construct(_begin + i++, *it);
		}
		_size = offset_size;
	}

	void
	assign(size_type n, const value_type& val)
	{
		if (n == 0)
			return ;
		if (n > _capacity)
		{
			size_type new_capacity = n; 
			this->__destroy_begin();
			_begin = _alloc.allocate(n);
			for (size_type i = 0; i < n; i++)
				_alloc.construct(_begin + i, val);
			_capacity = new_capacity;
		}
		else
		{	
			for (size_type i = 0; i < _size; i++)
				_alloc.destroy(_begin + i);
			for (size_type i = 0; i < n; i++)
				_alloc.construct(_begin + i, val);
		}
		_size = n;
	}

	void
	push_back(const value_type& val)
	{
		if (_capacity == 0)
		{
			_begin = _alloc.allocate(1);	
			_capacity++;
		}
		if (_size == _capacity)
			this->__realloc_begin(_capacity * 2);
		_alloc.construct(_begin + _size, val);
		_size++;
	}

	void
	pop_back()
	{
		_size--;
		_alloc.destroy(_begin + _size);
	}

	iterator
	insert(iterator position, const value_type& val)
	{
		pointer insert_location = NULL;
		if (_size == _capacity)
		{
			size_type new_capacity = __custom_cap(_size + 1);
			pointer new_begin = _alloc.allocate(new_capacity);
			iterator it = this->begin();
			size_type i = 0;
			while (it != position)
			{
				_alloc.construct(new_begin + i, _begin[i]);
				i++;
				it++;
			}
			insert_location = new_begin + i;
			new_begin[i++] = val;
			while (i < _size + 1)
			{
				_alloc.construct(new_begin + i, _begin[i - 1]);
				i++;
			}
			this->__destroy_begin();
			_begin = new_begin;
			_capacity = new_capacity;
		}
		else
		{
			iterator it = this->end();
			size_type i = _size;
			while (it != position)
			{
				_alloc.construct(_begin + i, _begin[i - 1]);
				_alloc.destroy(_begin + i - 1);
				i--;
				it--;
			}
			_alloc.construct(_begin + i, val);
			insert_location = _begin + i;
		}
		_size++;
		return iterator(insert_location);
	}

	void
	insert(iterator position, size_type n, const value_type& val)
	{
		if (n == 0)
			return ;
		size_type new_size = _size + n;
		if (new_size > _capacity)
		{
			size_type new_capacity = __custom_cap(new_size);
			pointer new_begin = _alloc.allocate(new_capacity);
			iterator it = this->begin();
			size_type i = 0;
			while (it != position)
			{
				_alloc.construct(new_begin + i, _begin[i]);
				i++;
				it++;
			}
			size_type j;
			for (j = 0; j < n; j++)
				_alloc.construct(new_begin + i + j, val);
			while (i + j < new_size)
			{
				_alloc.construct(new_begin + i + j, _begin[i]);
				i++;
			}
			this->__destroy_begin();
			_begin = new_begin;
			_capacity = new_capacity;
		}
		else
		{
			iterator it = this->begin();
			int i = 0;
			while (it != position)
			{
				it++;
				i++;
			}
			int insert_idx = i;
			i = _size - 1;
			while (i >= insert_idx)
			{
				_alloc.construct(_begin + i + n, _begin[i]);
				_alloc.destroy(_begin + i);
				i--;
			}
			for (int j = 0; j < static_cast<int>(n); j++)
				_alloc.construct(_begin + insert_idx + j, val);
		}
		_size = new_size;
	}

	template <class Iter_type>
	void
	insert(iterator position, Iter_type first, Iter_type last,
			typename ft::enable_if<!ft::is_integral<Iter_type>::value>::type* = 0)
	{
		size_type n = __iter_offset(first, last);
		if (n == 0)
			return ;
		size_type new_size = _size + n;
		if (new_size > _capacity)
		{
			size_type new_capacity = __custom_cap(new_size);
			pointer new_begin = _alloc.allocate(new_capacity);
			iterator it = this->begin();
			size_type i = 0;
			while (it != position)
			{
				_alloc.construct(new_begin + i, _begin[i]);
				i++;
				it++;
			}
			size_type j = 0;
			Iter_type in_it = first;
			while (in_it != last)
			{
				_alloc.construct(new_begin + i + j, *in_it);
				in_it++;
				j++;
			}
			while (i + j < new_size)
			{
				_alloc.construct(new_begin + i + j, _begin[i]);
				i++;
			}
			this->__destroy_begin();
			_begin = new_begin;
			_capacity = new_capacity;
		}
		else
		{
			iterator it = this->begin();
			int i = 0;
			while (it != position)
			{
				it++;
				i++;
			}
			int insert_idx = i;
			i = _size - 1;
			while (i >= insert_idx)
			{
				_alloc.construct(_begin + i + n, _begin[i]);
				_alloc.destroy(_begin + i);
				i--;
			}
			Iter_type in_it = first;
			int j = 0;
			while (in_it != last)
			{
				_alloc.construct(_begin + insert_idx + j, *in_it);
				in_it++;
				j++;
			}
		}
		_size = new_size;
	}

	iterator erase(iterator position)
	{
		if (position == this->end())
			return position;
		size_type i = position - begin();	
		_alloc.destroy(_begin + i);
		iterator it = position;
		while (it != this->end() - 1)
		{
			_alloc.construct(_begin + i, _begin[i + 1]);
			_alloc.destroy(_begin + i + 1);
			i++;
			it++;
		}
		_size--;
		return position;
	}

	iterator erase(iterator first, iterator last)
	{
		if (first == last)
			return first;
		iterator it = this->begin();
		size_type i = 0;
		while (it != first)
		{	
			i++;
			it++;
		}
		size_type j = 0;
		while (it != last)
		{
			_alloc.destroy(_begin + i + j);
			j++;
			it++;
		}
		while (it != this->end())
		{
			_alloc.construct(_begin + i, _begin[i + j]);
			_alloc.destroy(_begin + i + j);
			i++;
			it++;
		}
		_size -= j;
		return first;
	}
	
	void
	swap(vector<value_type>& x)
	{
		std::swap(this->_alloc, x._alloc);
		std::swap(this->_capacity, x._capacity);
		std::swap(this->_begin, x._begin);
		std::swap(this->_size, x._size);
	}

	void
	clear()
	{
		for (size_type i = 0; i < _size; i++)
			_alloc.destroy(_begin + i);
		_size = 0;
	}

	reference
	operator[](unsigned int offset)
	{
		return reference(*(_begin + offset));
	}

	const_reference
	operator[](unsigned int offset) const
	{
		reference(*(_begin + offset));
	}

	iterator
	begin(void)
	{
		iterator it(_begin);
		return it;
	}

	const_iterator
	begin(void) const
	{
		const_iterator it(_begin);
		return it;
	}

	iterator
	end(void)
	{
		iterator it(_begin + _size);
		return it;
	}

	const_iterator
	end(void) const
	{
		const_iterator it(_begin + _size);
		return it;
	}

	reverse_iterator
	rbegin(void)
	{
		reverse_iterator it(_begin + _size);
		return it;
	}

	const_reverse_iterator
	rbegin(void) const
	{
		const_reverse_iterator it(_begin + _size);
		return it;
	}

	reverse_iterator
	rend(void)
	{
		reverse_iterator it(_begin);
		return it;
	}

	const_reverse_iterator
	rend(void) const
	{
		const_reverse_iterator it(_begin);
		return it;
	}

	pointer
	data(void)
	{ return _begin; }

	reference
	at(size_type pos)
	{
		if (pos >= _size)
			throw std::out_of_range("vector");
		return _begin[pos];
	}

	const_reference
	at(size_type pos) const
	{
		if (pos >= _size)
			throw std::out_of_range("vector");
		return _begin[pos];
	}

	reference
	front()
	{ return _begin[0]; }

	const_reference
	front() const
	{ return _begin[0]; }

	reference
	back()
	{ return _begin[_size - 1]; }

	const_reference
	back() const
	{ return _begin[_size - 1]; }

	allocator_type
	get_allocator(void) const
	{ return _alloc; }

private:
	template <class Iter_type>
	difference_type
	__iter_offset(Iter_type first, Iter_type last)
	{
		difference_type d = 0;
		for (Iter_type it = first; it != last; it++) 
			d++;
		return d;	
	}

	size_type
	__custom_cap(size_type size)
	{	
		if (size > _capacity * 2)
			return size;
		if (_capacity > 0)
			return _capacity * 2;
		return 1;
	}

	void
	__realloc_begin(size_type new_capacity)
	{
		pointer new_begin = _alloc.allocate(new_capacity);	
		for (size_type i = 0; i < _size; i++)
			_alloc.construct(new_begin + i, _begin[i]);
		this->__destroy_begin();
		_begin = new_begin;
		_capacity = new_capacity;
	}

	void
	__destroy_begin(void)
	{
		if (_capacity == 0)
			return ;
		for (size_type i = 0; i < _size; i++)
			_alloc.destroy(_begin + i);
		_alloc.deallocate(_begin, _capacity);
	}

};

template< class T, class Alloc >
void
swap(ft::vector<T,Alloc>& lhs, ft::vector<T,Alloc>& rhs)
{ lhs.swap(rhs); }

template <class T, class Alloc>
bool operator==(const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs)
{
	const typename vector<T, Alloc>::size_type __sz = lhs.size();
    return __sz == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T, class Alloc>
bool operator!=(const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs)
{ return !(lhs == rhs); }

template <class T, class Alloc>
bool operator<=(const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs)
{
	return !(rhs < lhs);
}

template <class T, class Alloc>
bool operator<(const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs)
{
	return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T, class Alloc>
bool operator>=(const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs)
{
	return !(lhs < rhs);
}

template <class T, class Alloc>
bool operator>(const ft::vector<T,Alloc>& lhs, const ft::vector<T,Alloc>& rhs)
{
	return rhs < lhs;
}

}
#endif
