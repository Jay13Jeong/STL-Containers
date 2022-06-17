#ifndef MAP_CLASS_HPP
# define MAP_CLASS_HPP

# include "rbt.hpp"
# include "iterator_tree.hpp"
# include "reverse_iterator.hpp"

namespace ft
{
template < class Key, class T, class Compare = std::less<Key>, class Alloc = std::allocator< ft::pair<const Key,T> > >
class map
{
public:
	typedef Key											key_type;
	typedef T											mapped_type;
	typedef ft::pair<const key_type, mapped_type>			value_type;
	typedef Compare										key_compare;
	class	value_compare
	{
		friend class map;

		public:
		value_compare() : comp(Compare()) { };

		protected:
		Compare comp;
		value_compare(Compare c) : comp(c) { };

		public:
		typedef bool		result_type;
		typedef value_type	first_argument_type;
		typedef value_type	second_argument_type;
		bool	operator()(const value_type &x, const value_type &y) const {
			return comp(x.first, y.first);
		}
	};

	typedef Alloc										allocator_type;
	typedef typename allocator_type::reference			reference;
	typedef typename allocator_type::const_reference	const_reference;
	typedef typename allocator_type::pointer			pointer;
	typedef typename allocator_type::const_pointer		const_pointer;
	typedef ft::rbtNode<value_type>						node_type;
	typedef node_type*									node_ptr;

	typedef ptrdiff_t									difference_type;
	typedef size_t										size_type;

	typedef ft::iter_tree<value_type, node_type>			iterator;
	typedef ft::iter_tree<const value_type, node_type>		const_iterator;
	typedef ft::reverse_iterator<iterator>				reverse_iterator;
	typedef ft::reverse_iterator<const_iterator>		const_reverse_iterator;

	explicit map(const key_compare &comp = key_compare(),
			const allocator_type &alloc = allocator_type());
	template <class Ite>
	map(typename ft::enable_if<!ft::is_integral<Ite>::value, Ite>::type first,
			Ite last, const key_compare &comp = key_compare(),
			const allocator_type &alloc = allocator_type());
	map(const map &src);
	virtual ~map(void);

	map	&operator=(map const &rhs);

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

	mapped_type	&operator[](const key_type &k);

	ft::pair<iterator, bool>	insert(const value_type &val);
	iterator					insert(iterator position, const value_type &val);
	template <class Ite> void	insert(Ite first, Ite last);

	void		erase(iterator position);
	size_type	erase(const key_type &k);
	void		erase(iterator first, iterator last);

	void		swap(map &x);
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
		this->_tree.print_nodes_map();
	}

private:
	typedef value_compare		vc;
	typedef ft::rbt<value_type, vc, allocator_type>	tree_type;
	tree_type				_tree;
	key_compare				_key_cmp;

};

template <class Key, class T, class Compare, class Alloc>
map<Key, T, Compare, Alloc>::map(const key_compare &comp, const allocator_type \
		&alloc) : _key_cmp(comp)
{
	this->_tree._comp = value_compare(comp);
	this->_tree._alloc = alloc;
}

template <class Key, class T, class Compare, class Alloc> template <class Ite>
map<Key, T, Compare, Alloc>::map(
	typename ft::enable_if<!ft::is_integral<Ite>::value, Ite>::type first,
	Ite last, const key_compare &comp, const allocator_type &alloc) : \
		_key_cmp(comp)
{
	this->_tree._comp = value_compare(comp);;
	this->_tree._alloc = alloc;
	this->insert(first, last);
}

template<class Key, class T, class Compare, class Alloc>
map<Key, T, Compare, Alloc>::map(map const &src) : \
		_key_cmp(src._key_cmp)
{
	this->_tree._comp = src._tree.value_comp();
	this->_tree._alloc = src._tree.__alloc();
	insert(src.begin(), src.end());
}

template<class Key, class T, class Compare, class Alloc>
map<Key, T, Compare, Alloc>::~map(void) {
	this->clear();
}

template<class Key, class T, class Compare, class Alloc>
map<Key, T, Compare, Alloc>&
map<Key, T, Compare, Alloc>::operator=(map const &rhs) {
	if (this == &rhs)
		return (*this);
	this->_key_cmp = rhs._key_cmp;
	this->clear();
	this->_tree.value_comp() = rhs._tree.value_comp();
	this->_tree.__alloc() = rhs._tree.__alloc();
	this->insert(rhs.begin(), rhs.end());
	return (*this);
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::iterator
map<Key, T, Compare, Alloc>::begin(void) {
	return iterator(_tree.begin());
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::const_iterator
map<Key, T, Compare, Alloc>::begin(void) const {
	return const_iterator(_tree.begin());
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::iterator
map<Key, T, Compare, Alloc>::end(void) {
	return iterator(_tree.end());
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::const_iterator
map<Key, T, Compare, Alloc>::end(void) const {
	return const_iterator(_tree.end());
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::reverse_iterator
map<Key, T, Compare, Alloc>::rbegin(void) {
	return reverse_iterator(_tree.end());
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::const_reverse_iterator
map<Key, T, Compare, Alloc>::rbegin(void) const {
	return const_reverse_iterator(_tree.end());
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::reverse_iterator
map<Key, T, Compare, Alloc>::rend(void) {
	return reverse_iterator(_tree.begin());
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::const_reverse_iterator
map<Key, T, Compare, Alloc>::rend(void) const {
	return const_reverse_iterator(_tree.begin());
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::size_type
map<Key, T, Compare, Alloc>::size(void) const {
	return this->_tree.size();
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::size_type
map<Key, T, Compare, Alloc>::max_size(void) const {
	return this->_tree.max_size();
}

template<class Key, class T, class Compare, class Alloc>
bool	map<Key, T, Compare, Alloc>::empty(void) const {
	return (this->_tree.size() == 0);
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::mapped_type&
map<Key, T, Compare, Alloc>::operator[](const key_type &k)
{
	return (this->insert(ft::make_pair(k, mapped_type()))).first->second;
}

template<class Key, class T, class Compare, class Alloc>
ft::pair<typename map<Key, T, Compare, Alloc>::iterator, bool>
map<Key, T, Compare, Alloc>::insert(const value_type &val) {
	ft::pair<iterator, bool> res;

	res.first = this->find(val.first);
	res.second = res.first == end();
	if (res.second)
		res.first = iterator(this->_tree.insert(val));
	return (res);
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::iterator
map<Key, T, Compare, Alloc>::insert(iterator position, const value_type &val) {
	if (position != position )
		;
	return this->insert(val).first;
}

template<class Key, class T, class Compare, class Alloc> template <class Ite>
void	map<Key, T, Compare, Alloc>::insert(Ite first, Ite last) {
	while (first != last)
	{
		this->insert(*first++);
	}
}

template<class Key, class T, class Compare, class Alloc>
void	map<Key, T, Compare, Alloc>::erase(iterator position)
{
	this->_tree.delete_node(*position);
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::size_type
map<Key, T, Compare, Alloc>::erase(const key_type &k)
{
	return this->_tree.delete_node(ft::make_pair(k, mapped_type()));
}

template<class Key, class T, class Compare, class Alloc>
void	map<Key, T, Compare, Alloc>::erase(iterator first, iterator last)
{
	while (first != last)
		this->_tree.delete_node(*first++);
}

template<class Key, class T, class Compare, class Alloc>
void	map<Key, T, Compare, Alloc>::swap(map &x) {
	this->_tree.swap(x._tree);
}

template<class Key, class T, class Compare, class Alloc>
void	map<Key, T, Compare, Alloc>::clear(void)
{
	this->_tree.clear();
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::key_compare
map<Key, T, Compare, Alloc>::key_comp(void) const {
	return (key_compare());
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::value_compare
map<Key, T, Compare, Alloc>::value_comp(void) const {
	return (value_compare(key_compare()));
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::iterator
map<Key, T, Compare, Alloc>::find(const key_type &k)
{
	return iterator(this->_tree.find(ft::make_pair(k, mapped_type())));
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::const_iterator
map<Key, T, Compare, Alloc>::find(const key_type &k) const
{
	return const_iterator(this->_tree.find(ft::make_pair(k, mapped_type())));
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::size_type
map<Key, T, Compare, Alloc>::count(const key_type &k) const
{
	return !(this->_tree.find(ft::make_pair(k, mapped_type()))->_is_nul);
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::iterator
map<Key, T, Compare, Alloc>::lower_bound(const key_type &k) {
	iterator it = this->begin(), ite = this->end();
	//k값 또는 바로 직전 주소 반환.
	while (it != ite)
	{
		if (!this->_key_cmp(it->first, k))
			break;
		++it;
	}
	return (it);
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::const_iterator
map<Key, T, Compare, Alloc>::lower_bound(const key_type &k) const {
	const_iterator it = this->begin(), ite = this->end();

	while (it != ite)
	{
		if (!this->_key_cmp(it->first, k))
			break;
		++it;
	}
	return (it);
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::iterator
map<Key, T, Compare, Alloc>::upper_bound(const key_type &k) {
	iterator it = this->begin(), ite = this->end();
	//k값 또는 바로 직후 주소 반환.
	while (it != ite)
	{
		if (this->_key_cmp(k, it->first))
			break;
		++it;
	}
	return (it);
}

template<class Key, class T, class Compare, class Alloc>
typename map<Key, T, Compare, Alloc>::const_iterator
map<Key, T, Compare, Alloc>::upper_bound(const key_type &k) const {
	const_iterator it = this->begin(), ite = this->end();

	while (it != ite)
	{
		if (this->_key_cmp(k, it->first))
			break;
		++it;
	}
	return (it);
}

template<class Key, class T, class Compare, class Alloc>
ft::pair<typename map<Key, T, Compare, Alloc>::const_iterator, typename map<Key, T, Compare, Alloc>::const_iterator>
map<Key, T, Compare, Alloc>::equal_range(const key_type &k) const {
	return ft::pair<const_iterator, const_iterator>(lower_bound(k),upper_bound(k));
}

template<class Key, class T, class Compare, class Alloc>
ft::pair<typename map<Key, T, Compare, Alloc>::iterator, typename map<Key, T, Compare, Alloc>::iterator>
map<Key, T, Compare, Alloc>::equal_range(const key_type &k) {
	return ft::pair<iterator, iterator>(lower_bound(k),upper_bound(k));
}

template <class Key, class T, class Compare, class Alloc>
bool	operator==(const map<Key, T, Compare, Alloc> &lhs,
					const map<Key, T, Compare, Alloc> &rhs)
{
	return lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class Key, class T, class Compare, class Alloc>
bool	operator!=(const map<Key, T, Compare, Alloc> &lhs,
					const map<Key, T, Compare, Alloc> &rhs) {
	return !(lhs == rhs);
}

template <class Key, class T, class Compare, class Alloc>
bool	operator< (const map<Key, T, Compare, Alloc> &lhs,
					const map<Key, T, Compare, Alloc> &rhs) {
	return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class Key, class T, class Compare, class Alloc>
bool	operator<=(const map<Key, T, Compare, Alloc> &lhs,
					const map<Key, T, Compare, Alloc> &rhs) {
	return !(rhs < lhs);
}

template <class Key, class T, class Compare, class Alloc>
bool	operator> (const map<Key, T, Compare, Alloc> &lhs,
					const map<Key, T, Compare, Alloc> &rhs) {
	return (rhs < lhs);
}

template <class Key, class T, class Compare, class Alloc>
bool	operator>=(const map<Key, T, Compare, Alloc> &lhs,
					const map<Key, T, Compare, Alloc> &rhs) {
	return !(lhs < rhs);
}

template <class Key, class T, class Compare, class Alloc>
void	swap(map<Key, T, Compare, Alloc> &x, map<Key, T, Compare, Alloc> &y) {
	x.swap(y);
}

}

#endif
