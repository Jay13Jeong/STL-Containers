#ifndef SET_CLASS_HPP
# define SET_CLASS_HPP

# include "rbt.hpp"
# include "iterator_tree.hpp"
# include "reverse_iterator.hpp"

namespace ft
{
template < class Key, class Compare = std::less<Key>, class Alloc = std::allocator< Key> >
class set
{
public:
	typedef Key											key_type;
	typedef Key											value_type;
	typedef Compare										key_compare;
	typedef Compare									value_compare;

	typedef Alloc										allocator_type;
	typedef typename allocator_type::reference			reference;
	typedef typename allocator_type::const_reference	const_reference;
	typedef typename allocator_type::pointer			pointer;
	typedef typename allocator_type::const_pointer		const_pointer;

	typedef ptrdiff_t									difference_type;
	typedef size_t										size_type;

	typedef ft::iter_tree<const value_type,ft::rbtNode<value_type> >			iterator;
	typedef ft::iter_tree<const value_type,ft::rbtNode<value_type> >		const_iterator;
	typedef ft::reverse_iterator<iterator>				reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>		const_reverse_iterator;

	explicit set(const key_compare &comp = key_compare(),
			const allocator_type &alloc = allocator_type());
	template <class Ite>
	set(typename ft::enable_if<!ft::is_integral<Ite>::value, Ite>::type first,
			Ite last, const key_compare &comp = key_compare(),
			const allocator_type &alloc = allocator_type());
	set(const set &src);
	virtual ~set(void);

	set	&operator=(set const &rhs);

	iterator				begin(void);
	const_iterator			begin(void) const;
	iterator				end(void);
	const_iterator			end(void) const;

	reverse_iterator		rbegin(void);
	const_reverse_iterator	rbegin(void) const;
	reverse_iterator		rend(void);
	const_reverse_iterator	rend(void) const;

	size_type	size(void) const;
	size_type	max_size(void) const;
	bool		empty(void) const;

	ft::pair<iterator, bool>	insert(const value_type &val);
	iterator					insert(iterator position, const value_type &val);
	template <class Ite> void	insert(Ite first, Ite last);

	void		erase(iterator position);
	size_type	erase(const key_type &k);
	void		erase(iterator first, iterator last);

	void		swap(set &x);
	void		clear(void);

	key_compare		key_comp(void) const;
	value_compare	value_comp(void) const;

	iterator		find(const key_type &k);
	const_iterator	find(const key_type &k) const;
	size_type		count(const key_type &k) const;

	iterator		lower_bound(const key_type &k);
	const_iterator	lower_bound(const key_type &k) const;
	iterator		upper_bound(const key_type &k);
	const_iterator	upper_bound(const key_type &k) const;
	pair<const_iterator,const_iterator>	equal_range(const key_type &k) const;
	pair<iterator,iterator>				equal_range(const key_type &k);


	void print_node() const
	{
		this->_tree.print_nodes();
	}

private:
	typedef value_compare		vc;
	typedef ft::rbt<value_type, vc, allocator_type>	tree_type;
	tree_type				_tree;
	key_compare				_key_cmp;

};

template <class Key, class Compare, class Alloc>
set<Key, Compare, Alloc>::set(const key_compare &comp, const allocator_type \
		&alloc) : _key_cmp(comp)
{
	this->_tree._comp = value_compare(comp);
	this->_tree._alloc = alloc;
}

template <class Key, class Compare, class Alloc> template <class Ite>
set<Key, Compare, Alloc>::set(
	typename ft::enable_if<!ft::is_integral<Ite>::value, Ite>::type first,
	Ite last, const key_compare &comp, const allocator_type &alloc) : \
		_key_cmp(comp)
{
	this->_tree._comp = value_compare(comp);;
	this->_tree._alloc = alloc;
	this->insert(first, last);
}

template<class Key, class Compare, class Alloc>
set<Key, Compare, Alloc>::set(set const &src) : \
		_key_cmp(src._key_cmp)
{
	this->_tree._comp = src._tree.value_comp();
	this->_tree._alloc = src._tree.__alloc();
	insert(src.begin(), src.end());
}

template<class Key, class Compare, class Alloc>
set<Key, Compare, Alloc>::~set(void) {
	this->clear();
}

template<class Key, class Compare, class Alloc>
set<Key, Compare, Alloc>&
set<Key, Compare, Alloc>::operator=(set const &rhs) {
	if (this == &rhs)
		return (*this);
	this->_key_cmp = rhs._key_cmp;
	this->clear();
	this->_tree.value_comp() = rhs._tree.value_comp();
	this->_tree.__alloc() = rhs._tree.__alloc();
	this->insert(rhs.begin(), rhs.end());
	return (*this);
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::iterator
set<Key, Compare, Alloc>::begin(void) {
	return iterator(_tree.begin());
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::const_iterator
set<Key, Compare, Alloc>::begin(void) const {
	return const_iterator(_tree.begin());
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::iterator
set<Key, Compare, Alloc>::end(void) {
	return iterator(_tree.end());
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::const_iterator
set<Key, Compare, Alloc>::end(void) const {
	return const_iterator(_tree.end());
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::reverse_iterator
set<Key, Compare, Alloc>::rbegin(void) {
	return reverse_iterator(_tree.end());
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::const_reverse_iterator
set<Key, Compare, Alloc>::rbegin(void) const {
	return const_reverse_iterator(_tree.end());
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::reverse_iterator
set<Key, Compare, Alloc>::rend(void) {
	return reverse_iterator(_tree.begin());
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::const_reverse_iterator
set<Key, Compare, Alloc>::rend(void) const {
	return const_reverse_iterator(_tree.begin());
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::size_type
set<Key, Compare, Alloc>::size(void) const {
	return this->_tree.size();
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::size_type
set<Key, Compare, Alloc>::max_size(void) const {
	return this->_tree.max_size();
}

template<class Key, class Compare, class Alloc>
bool	set<Key, Compare, Alloc>::empty(void) const {
	return (this->_tree.size() == 0);
}

template<class Key, class Compare, class Alloc>
ft::pair<typename set<Key, Compare, Alloc>::iterator, bool>
set<Key, Compare, Alloc>::insert(const value_type &val) {
	ft::pair<iterator, bool> res;

	res.first = this->find(val);
	res.second = res.first == end();
	if (res.second)
		res.first = iterator(this->_tree.insert(val));
	return (res);
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::iterator
set<Key, Compare, Alloc>::insert(iterator position, const value_type &val) {
	if (position != position )
		;
	return iterator(this->_tree.insert(val));
}

template<class Key, class Compare, class Alloc> template <class Ite>
void	set<Key, Compare, Alloc>::insert(Ite first, Ite last) {
	while (first != last)
	{
		this->_tree.insert(*first++);
	}
}

template<class Key, class Compare, class Alloc>
void	set<Key, Compare, Alloc>::erase(iterator position)
{
	this->_tree.delete_node(*position);
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::size_type
set<Key, Compare, Alloc>::erase(const key_type &k)
{
	return this->_tree.delete_node(k);
}

template<class Key, class Compare, class Alloc>
void	set<Key, Compare, Alloc>::erase(iterator first, iterator last)
{
	while (first != last)
	{
		this->_tree.delete_node(*first++);
	}
		
}

template<class Key, class Compare, class Alloc>
void	set<Key, Compare, Alloc>::swap(set &x) {
	this->_tree.swap(x._tree);
}

template<class Key, class Compare, class Alloc>
void	set<Key, Compare, Alloc>::clear(void)
{
	this->_tree.clear();
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::key_compare
set<Key, Compare, Alloc>::key_comp(void) const {
	return (key_compare());
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::value_compare
set<Key, Compare, Alloc>::value_comp(void) const {
	return (value_compare(key_compare()));
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::iterator
set<Key, Compare, Alloc>::find(const key_type &k)
{
	return iterator(this->_tree.find(k));
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::const_iterator
set<Key, Compare, Alloc>::find(const key_type &k) const
{
	return const_iterator(this->_tree.find(k));
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::size_type
set<Key, Compare, Alloc>::count(const key_type &k) const
{
	return !(this->_tree.find(k)->_is_nul);
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::iterator
set<Key, Compare, Alloc>::lower_bound(const key_type &k) {
	iterator it = this->begin(), ite = this->end();

	while (it != ite)
	{
		if (!this->_key_cmp(*it, k))
			break;
		++it;
	}
	return (it);
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::const_iterator
set<Key, Compare, Alloc>::lower_bound(const key_type &k) const {
	const_iterator it = this->begin(), ite = this->end();

	while (it != ite)
	{
		if (!this->_key_cmp(*it, k))
			break;
		++it;
	}
	return (it);
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::iterator
set<Key, Compare, Alloc>::upper_bound(const key_type &k) {
	iterator it = this->begin(), ite = this->end();

	while (it != ite)
	{
		if (this->_key_cmp(k, *it))
			break;
		++it;
	}
	return (it);
}

template<class Key, class Compare, class Alloc>
typename set<Key, Compare, Alloc>::const_iterator
set<Key, Compare, Alloc>::upper_bound(const key_type &k) const {
	const_iterator it = this->begin(), ite = this->end();

	while (it != ite)
	{
		if (this->_key_cmp(k, *it))
			break;
		++it;
	}
	return (it);
}

template<class Key, class Compare, class Alloc>
ft::pair<typename set<Key, Compare, Alloc>::const_iterator, typename set<Key, Compare, Alloc>::const_iterator>
set<Key, Compare, Alloc>::equal_range(const key_type &k) const
{
	return ft::pair<const_iterator, const_iterator>(lower_bound(k),upper_bound(k));
}

template<class Key, class Compare, class Alloc>
ft::pair<typename set<Key, Compare, Alloc>::iterator, typename set<Key, Compare, Alloc>::iterator>
set<Key, Compare, Alloc>::equal_range(const key_type &k) {
	return ft::pair<iterator, iterator>(lower_bound(k),upper_bound(k));
}

template <class Key, class Compare, class Alloc>
bool	operator==(const set<Key, Compare, Alloc> &lhs,
					const set<Key, Compare, Alloc> &rhs)
{
	return lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class Key, class Compare, class Alloc>
bool	operator!=(const set<Key, Compare, Alloc> &lhs,
					const set<Key, Compare, Alloc> &rhs) {
	return !(lhs == rhs);
}

template <class Key, class Compare, class Alloc>
bool	operator< (const set<Key, Compare, Alloc> &lhs,
					const set<Key, Compare, Alloc> &rhs) {
	return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class Key, class Compare, class Alloc>
bool	operator<=(const set<Key, Compare, Alloc> &lhs,
					const set<Key, Compare, Alloc> &rhs) {
	return !(rhs < lhs);
}

template <class Key, class Compare, class Alloc>
bool	operator> (const set<Key, Compare, Alloc> &lhs,
					const set<Key, Compare, Alloc> &rhs) {
	return (rhs < lhs);
}

template <class Key, class Compare, class Alloc>
bool	operator>=(const set<Key, Compare, Alloc> &lhs,
					const set<Key, Compare, Alloc> &rhs) {
	return !(lhs < rhs);
}

template <class Key, class Compare, class Alloc>
void	swap(set<Key, Compare, Alloc> &x, set<Key, Compare, Alloc> &y) {
	x.swap(y);
}

}

#endif
