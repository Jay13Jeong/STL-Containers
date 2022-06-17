#ifndef RBT_CLASS_HPP
# define RBT_CLASS_HPP

namespace ft 
{
//rbt노드 구조체
template <typename T>
struct	rbtNode
{
	bool		_is_black;
	bool		_is_nul;
	T			_data;
	rbtNode		*_parent;
	rbtNode		*_left;
	rbtNode		*_right;

	rbtNode(const T &data_ = T()) : _is_black(false), _is_nul(false), _data(data_), _parent(NULL), _left(NULL), _right(NULL) {};
};

template<typename T, typename Comp = std::less<T>, typename Alloc = std::allocator<T> >
class rbt
{
public:
	typedef rbtNode<T>		node;
	node	*_root;
	Comp	_comp;
	Alloc	_alloc;

private:
	node	*_inserted_node;
	node	*_end_node;
	size_t	_size;
	size_t	_max_size;
	typename Alloc::template rebind<node>::other _node_alloc;

    node	*most_right(node *node) const {
        while (node->_right != NULL)
            node = node->_right;
        return (node);
    }

    node	*most_left(node *node) const {
        while (node->_left != NULL)
            node = node->_left;
        return (node);
    }

public:
	rbt(const Comp& comp_ = Comp(), const Alloc& alloc_ = Alloc()) : _root(NULL), _comp(comp_), _alloc(alloc_), _inserted_node(NULL) , _size(0)
	{
		size_t div = sizeof( node ) / 2;
		if (div == 0)
			div = 1;
		this->_max_size = std::numeric_limits<ptrdiff_t>::max() / div;

		node *nul		=  _node_alloc.allocate(1);
		_alloc.construct(&nul->_data, T());
		nul->_is_nul = true;
		nul->_is_black = true;
		_end_node = nul;
	}

	~rbt() { 
		clear();
		_alloc.destroy(&_end_node->_data);
		_node_alloc.deallocate(_end_node, 1);
		};

	node*
	begin() const {
		if (_size == 0)
			return _end_node;
		return this->most_left(this->_root);
		};

	node*
	end() const
	{
		return _end_node;
	}

	node*
	rbegin() const {
		if (_size == 0)
			return _end_node;		
		return this->most_right(this->_root);
		};

	node*
	rend() const
	{
		return _end_node;
	}

	//일반 이진트리처럼 데이터를 넣고 rbt의 조건으로 검사 및 수정 한다.
	node*
	insert(const T& data_)
	{
		if (_root != NULL)
		{
			_end_node->_left = NULL;
			_end_node->_right = NULL;
			_root->_parent = NULL;
		}
		_root = insert_value(this->_root, data_, NULL);
        insert_case1(this->_inserted_node);
		_root->_parent = _end_node;
		_end_node->_left = _root;
		_end_node->_right = _root;
		return _inserted_node;
	}

	// 이진탐색으로 삽입할 자리를 찾고 노드객체를 생성.
	node*
    insert_value(node *node_, const T& data_, node *parent_node_)
	{
        if (node_ == NULL)
		{
			node *n					= _node_alloc.allocate(1);
			_alloc.construct(&n->_data, data_);
            n->_parent				= parent_node_;
			n->_left				= NULL;
			n->_right				= NULL;
            this->_inserted_node	= n;
			++this->_size;
			return n;
		};
		if (_comp(data_,node_->_data))
			node_->_left = this->insert_value(node_->_left, data_, node_);
		else if (_comp(node_->_data, data_))
			node_->_right = this->insert_value(node_->_right, data_, node_);
        return node_;
	};

	node*
	find(const T& search_key) const
	{
        return this->find_value(this->_root, search_key);
	};

	node*
	find_value(node *node_, const T& search_key) const
	{
        if (node_ == NULL)
            return _end_node;
        else if (_comp(node_->_data, search_key))
            return this->find_value(node_->_right, search_key);
        else if (_comp(search_key, node_->_data))
            return this->find_value(node_->_left, search_key);
		else
			return node_;
	};

	// 조부모 노드 찾기
    node*
	find_grandparent_node(node *node_) const
	{
        if ((node_ != NULL) && (node_->_parent != NULL))
            return node_->_parent->_parent;
        return NULL;
	};

    // 삼촌 노드 찾기
    node*
	find_uncle_node(node *node_) const
	{
        node *grandparent_node = this->find_grandparent_node(node_);
        if (grandparent_node == NULL)
            return NULL;
        else if (node_->_parent == grandparent_node->_left)
            return grandparent_node->_right;
        return grandparent_node->_left;
	};

    // case1. 루트 노드는 항상 블랙
	void
	inline
	insert_case1(node *node_)
	{
        if (node_->_parent == NULL)
            node_->_is_black = true;
        else
            this->insert_case2(node_);
	};
	   
    // case2. 부모 노드가 블랙이면 회전, 색변환등 수행 필요 x, 하지만 빨강색이라면 case3 수행
    void
	inline
	insert_case2(node *node_)
	{
        if (node_->_parent->_is_black == false)
            this->insert_case3(node_);
	};

    // case3. 부모노드, 삼촌노드 모두 빨강이라면 색변환 수행, 아닐경우 case4로 이동
    void
	insert_case3(node *node_)
	{
        node *uncle = this->find_uncle_node(node_);
    
        if (uncle != NULL && uncle->_is_black == false)
		{
            node_->_parent->_is_black = true;
            uncle->_is_black = true;
            node *grandparent = this->find_grandparent_node(node_);
            grandparent->_is_black = false;
            this->insert_case1(grandparent);
		}
		else //삼촌 노드가 없거나 부모가 블랙일때.
            this->insert_case4(node_);
	};
    
    // case4,5 회전 수행.
    void
	insert_case4(node *node_)
    {
        node *grandparent = this->find_grandparent_node(node_);
		node *rotated_node = NULL;
    

        if(node_ == node_->_parent->_right && node_->_parent == grandparent->_left)
        {
			//내가 오른쪽 자식이고, 부모가 왼쪽 자식일 때
			this->rotate_left(node_->_parent);
            rotated_node = node_->_left;
			this->insert_case5(rotated_node);
		}
        else if (node_ == node_->_parent->_left && node_->_parent == grandparent->_right)
        {
			//내가 왼쪽자식이고 부모가 오른쪽 자식일 때.
		    this->rotate_right(node_->_parent);
            rotated_node = node_->_right;
			this->insert_case5(rotated_node);
		}
		else
			this->insert_case5(node_);
	};

    void
	rotate_left(node *node_)
	{
        node *c = node_->_right;
        node *p = node_->_parent;
    
		//나의 오른쪽 자식의 왼쪽노드와 서로 링크.
        if (c->_left != NULL)
            c->_left->_parent = node_;
        node_->_right = c->_left;
		//나의 부모노드를 나의 오른쪽노드로 서로 링크.
        node_->_parent = c;
        c->_left = node_;
		//내가 원래 루트 노드였다면 root멤버변수 주소 수정.
		if (p == NULL)
			this->_root = c;
		//새로운 나의 부모를 기존 부모를 자식으로 서로 링크. 
        c->_parent = p;
        if (p != NULL)
		{
            if (p->_left == node_)
                p->_left = c;
            else
                p->_right = c;
		};
	};

    void
	rotate_right(node *node_)
	{
        node *c = node_->_left;
        node *p = node_->_parent;

		//나의 왼쪽 자식의 오른쪽쪽노드와 서로 링크.
        if (c->_right != NULL)
            c->_right->_parent = node_;
        node_->_left = c->_right;
		//나의 부모노드를 나의 왼쪽노드로 서로 링크.
        node_->_parent = c;
        c->_right = node_;
		//내가 원래 루트 노드였다면 root멤버변수 주소 수정.
		if (p == NULL)
			this->_root = c;
		//새로운 나의 부모를 기존 부모를 자식으로 서로 링크.
        c->_parent = p;
        if (p != NULL)
		{
            if (p->_right == node_)
                p->_right = c;
            else
                p->_left = c;
		};
	};
     
    void
	insert_case5(node *node_)
	{
        node *grandparent = this->find_grandparent_node(node_);
        node_->_parent->_is_black = true;
        grandparent->_is_black = false;

        if (node_ == node_->_parent->_left)
            this->rotate_right(grandparent);
        else
            this->rotate_left(grandparent);
	};

	bool
	delete_node(const T& data_)
	{
		
		node *target = find(data_);
		if (target == _end_node)
			return false;
		
		_end_node->_left = NULL;
		_end_node->_right = NULL;
		_root->_parent = NULL;

		if (target->_right != NULL)
		{
			node *close = most_left(target->_right);

			if (close->_right)
			{
				close->_right->_is_black = true;
				close->_is_black = false;
				rotate_left(close);
			}

			data_swap(target, close);
		}
		else if (target->_left != NULL)
		{
			node *close = most_right(target->_left);

			if (close->_left)
			{
				close->_left->_is_black = true;
				close->_is_black = false;
				rotate_right(close);
			}

			data_swap(target, close);
		}

		if (target->_parent == NULL)
			_root = NULL;
		else
		{
			node *sibling = find_sibling(target);

			if (target->_parent->_left == target)
				target->_parent->_left = NULL;
			else if (target->_parent->_right == target)
				target->_parent->_right = NULL;
			delete_case1(sibling);
		}

		if (_root != NULL)
			_root->_parent = _end_node;
		_end_node->_left = _root;
		_end_node->_right = _root;

		_alloc.destroy(&target->_data);
		_node_alloc.deallocate(target, 1);
		if (_size)
			--_size;
		return true;
	}

	void
	data_swap(node *n1, node *n2)
	{
		node temp;
		node temp2;
		bool p_path;
		bool p_path2;

		if (_root == n1)
			_root = n2;
		
		temp._is_black = n1->_is_black;
		temp._left = n1->_left;
		temp._parent = n1->_parent;
		temp._right = n1->_right;
		temp2._is_black = n2->_is_black;
		temp2._left = n2->_left;
		temp2._parent = n2->_parent;
		temp2._right = n2->_right;

		if (n1->_parent)
		{
			if (n1->_parent->_left == n1)
				p_path = true;
			else
				p_path = false;
		}
		if (n2->_parent)
		{
			if (n2->_parent->_left == n2)
				p_path2 = true;
			else
				p_path2 = false;
		}

		n1->_is_black = temp2._is_black;
		if (temp2._left == n1)
			n1->_left = n2;
		else
			n1->_left = temp2._left;
		if (temp2._parent == n1)
			n1->_parent = n2;
		else
			n1->_parent = temp2._parent;
		if (temp2._right == n1)
			n1->_right = n2;
		else
			n1->_right = temp2._right;

		n2->_is_black = temp._is_black;
		if (temp._left == n2)
			n2->_left = n1;
		else
			n2->_left = temp._left;
		if (temp._parent == n2)
			n2->_parent = n1;
		else
			n2->_parent = temp._parent;
		if (temp._right == n2)
			n2->_right = n1;
		else
			n2->_right = temp._right;
		
		if (n1->_left)
			n1->_left->_parent = n1;
		if (n1->_right)
			n1->_right->_parent = n1;
		if (n1->_parent)
		{
			if (p_path2)
				n1->_parent->_left = n1;
			else
				n1->_parent->_right = n1;
		}

		if (n2->_left)
			n2->_left->_parent = n2;
		if (n2->_right)
			n2->_right->_parent = n2;
		if (n2->_parent)
		{
			if (p_path)
				n2->_parent->_left = n2;
			else
				n2->_parent->_right = n2;
		}
	}

	//형제찾기.
	node*
	find_sibling(node *node_)
	{
		if (node_->_parent)
		{
			if (node_ == node_->_parent->_left)
				return node_->_parent->_right;
			if (node_ == node_->_parent->_right)
				return node_->_parent->_left;
		}
		return NULL;
	}

	void
	delete_case1(node *sibling)
	{
		if (sibling != NULL && sibling->_is_black == false)
		{
			sibling->_is_black = true;
			sibling->_parent->_is_black = false;
			if (sibling == sibling->_parent->_right)
				rotate_left(sibling->_parent);
			else
				rotate_right(sibling->_parent);
		}
		else if (sibling != NULL && sibling->_is_black == true)
			delete_case2(sibling);
	}

	void
	delete_case2(node *sibling)
	{
		//형제와 그 자식 모두 검정일 때.
		if (!(sibling->_left != NULL && !sibling->_left->_is_black) &&
			!(sibling->_right != NULL && !sibling->_right->_is_black))
		{
			sibling->_is_black = false;
			sibling->_parent->_is_black = true;
		}
		else
			delete_case3(sibling);
	}

	//형제의 자식에 레드가 있을 때.
	void
	delete_case3(node *sibling)
	{
		if (sibling == sibling->_parent->_right)
		{
			//가까운 조카가 레드일 때.
			if ((sibling->_left != NULL && sibling->_left->_is_black == false) &&
				!(sibling->_right != NULL && sibling->_right->_is_black == false))
			{
				sibling->_left->_is_black = true;
				sibling->_is_black = false;
				rotate_right(sibling);
				sibling = sibling->_parent;
			}
			if (sibling->_right != NULL && sibling->_right->_is_black == false)
			{
				sibling->_is_black = sibling->_parent->_is_black;
				sibling->_parent->_is_black = true;
				sibling->_right->_is_black = true;
				rotate_left(sibling->_parent);
			}
		}
		else
		{
			//깉은 상황, 형제 위치 반대.
			if ((sibling->_right != NULL && sibling->_right->_is_black == false) &&
				!(sibling->_left != NULL && sibling->_left->_is_black == false))
			{
				sibling->_right->_is_black = true;
				sibling->_is_black = false;
				rotate_left(sibling);
				sibling = sibling->_parent;
			}
			if (sibling->_left != NULL && sibling->_left->_is_black == false)
			{
				sibling->_is_black = sibling->_parent->_is_black;
				sibling->_parent->_is_black = true;
				sibling->_left->_is_black = true;
				rotate_right(sibling->_parent);
			}
		}
	}

	void
	clear()
	{
		_end_node->_left = NULL;
		_end_node->_right = NULL;
		if (this->_size != 0)
			tree_clear(this->_root);
		_inserted_node = NULL;
		_root = NULL;
		this->_size = 0;
	}

	//후위순회로 동적할당된 노드를 모두 해제.
	void
	tree_clear(node *node_) 
	{
		if (node_ == NULL)
			return ;
		this->tree_clear(node_->_left);
		this->tree_clear(node_->_right);
		_alloc.destroy(&node_->_data);
		_node_alloc.deallocate(node_, 1);
	};

	size_t
	size() const { return this->_size; }

	size_t
	max_size() const { return this->_max_size; }

	void
	swap(rbt<T, Comp, Alloc>& tree2)
	{
		std::swap(this->_inserted_node, tree2._inserted_node);
		std::swap(this->_root, tree2._root);
		std::swap(this->_size, tree2._size);
		std::swap(this->_alloc, tree2._alloc);
		std::swap(this->_comp, tree2._comp);
		std::swap(this->_end_node, tree2._end_node);
	}

	Comp value_comp() const { return _comp; }

	Alloc __alloc() const {return _alloc; }

	//중위순회로 노드 모두 출력.
	void
	print_nodes(void) const
	{
		check(this->_root);
	};

	void
	check(node *node_) const
	{
		if (node_->_left != NULL)
			check(node_->_left);
		if (node_->_parent != NULL)
			std::cout << "key: " << node_->_data << "\tparents: " << node_->_parent->_data << "\tcolor: " << (node_->_is_black ? "black" : "red") << std::endl;
		else
			std::cout << "key: " << node_->_data << "\tparents: " << node_->_parent << " \tcolor: " << (node_->_is_black ? "black" : "red") << std::endl;
		if (node_->_right != NULL)
			check(node_->_right);
	};

	void
	print_nodes_map(void) const
	{
		check2(this->_root);
	};

	void
	check2(node *node_) const
	{
		if (node_->_left != NULL)
			check2(node_->_left);
		if (node_->_parent != NULL)
			std::cout << "key: " << node_->_data.first << "\tv: " << node_->_data.second << "\tparents: " << node_->_parent->_data.first << "\tv: " << node_->_parent->_data.second << "\tcolor: " << (node_->_is_black ? "black" : "red") << std::endl;
		else
			std::cout << "key: " << node_->_data.first << "\tv: " << node_->_data.second<< "\tparents: " << node_->_parent << " \tcolor: " << (node_->_is_black ? "black" : "red") << std::endl;
		if (node_->_right != NULL)
			check2(node_->_right);
	};
};
}
#endif
