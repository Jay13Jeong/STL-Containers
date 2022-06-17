#ifndef ITERATOR_TREE_CLASS_HPP
# define ITERATOR_TREE_CLASS_HPP

# include "utils.hpp"

namespace ft
{
template <typename T, typename node_type>
class iter_tree
{
protected:
    node_type						*_node;
    iter_tree(node_type *src) { this->_node = src; }

public:
    typedef T						value_type;
    typedef ptrdiff_t				difference_type;
    typedef value_type&				reference;
    typedef value_type*				pointer;
    typedef std::bidirectional_iterator_tag	iterator_category;

private:
    node_type	*__most_right(node_type *node) {
        while (node->_right != NULL)
            node = node->_right;
        return (node);
    }

    node_type	*__most_left(node_type *node) {
        while (node->_left != NULL)
            node = node->_left;
        return (node);
    }

public:
    iter_tree(void) : _node(NULL) {}
    iter_tree(const iter_tree &src)  { *this = src; }
    virtual ~iter_tree(void) {}
    iter_tree	&operator=(iter_tree const &rhs)
	{
		if (this == &rhs)
			return (*this);
		this->_node = rhs._node;
		return (*this);
	}

    template <class U> bool	operator==(const iter_tree<U, node_type> &rhs) const
	{
		return (this->_node == rhs._node);
	}

    template <class U> bool	operator!=(const iter_tree<U, node_type> &rhs) const
	{
		return (this->_node != rhs._node);
	}

    iter_tree		&operator++(void)
	{
		if (this->_node->_right != NULL)
			this->_node = this->__most_left(this->_node->_right);
		else
		{
			node_type	*child = this->_node;

			this->_node = this->_node->_parent;
			while (this->_node && !this->_node->_is_nul && child == this->_node->_right)
			{
				child = this->_node;
				this->_node = this->_node->_parent;
			}
		}
		return (*this);
	}

    iter_tree		operator++(int)
	{
		iter_tree	tmp(*this);
		++(*this);
		return (tmp);
	}


    iter_tree		&operator--(void)
	{
		if (this->_node->_left != NULL)
			this->_node = this->__most_right(this->_node->_left);
		else
		{
			node_type	*child = this->_node;

			this->_node = this->_node->_parent;
			while (this->_node && !this->_node->_is_nul && child == this->_node->_left)
			{
				child = this->_node;
				this->_node = this->_node->_parent;
			}
		}
		return (*this);
	}

    iter_tree		operator--(int)
	{
		iter_tree	tmp(*this);
		--(*this);
		return (tmp);
	}

    reference	operator*(void) const
	{
		return (this->_node->_data);
	}

    pointer		operator->(void) const
	{
		return &this->operator*();
	}

    operator iter_tree<const T, node_type>(void) const
	{
        return iter_tree<const T, node_type>(this->_node);
    }

    template <class, class, class, class>
    friend class map;
	template <class, class, class>
	friend class set;

    template <class, class>
    friend class iter_tree;

};

}

#endif
