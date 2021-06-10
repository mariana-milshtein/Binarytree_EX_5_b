#pragma once
#include <queue>
#include <iostream>

namespace ariel {
    
};

template<typename T>
class BinaryTree {
private:
    struct Node {
        T data;
        Node *left, *right, *parent;
        
        Node(const T& data, Node* left, Node* right, Node* parent) : data(data) , left(left) , right(right) , parent(parent) {
        }
    };
    
public:
    BinaryTree() : root(nullptr) {
    }
    
    BinaryTree(const BinaryTree<T>& other) {
        copy(other.root);
    }
    
    BinaryTree(BinaryTree<T>&& other) noexcept {
        root = other.root;
        other.root = nullptr;
    }
    
    ~BinaryTree() {
        clean(root);    
    }
    
    BinaryTree& add_root(const T& data) {
        if(root == nullptr) {
            root = new Node(data, nullptr, nullptr, nullptr);
        }
        else {
            root->data = data;
        }
        return *this;    
    }
    
    BinaryTree& add_left(const T& parent, const T& newChild) {
        Node* p = find(parent);
        if(p == nullptr) {
            throw std::logic_error("First input is not in the tree");
        }
        if(p->left == nullptr) {
            p->left = new Node(newChild, nullptr, nullptr, p);
        }
        else {
            p->left->data = newChild;
        }
        return *this;  
    }
    
    BinaryTree& add_right(const T& parent, const T& newChild) {
        Node* p = find(parent);
        if(p == nullptr) {
            throw std::logic_error("First input is not in the tree");
        }
        if(p->right == nullptr) {
            p->right = new Node(newChild, nullptr, nullptr, p);
        }
        else {
            p->right->data = newChild;
        }
        return *this; 
    }
    
    BinaryTree& operator =(const BinaryTree<T>& other) noexcept {
        if (& other!=this)
        {
        clean(root);
        copy(other.root);
        }
        return *this;
    }
    
    BinaryTree& operator =(BinaryTree<T>&& other) noexcept {
        
        root = other.root;
        other.root = nullptr;
        return *this;
    }
    
    class iterator {  
    public:
        Node* current;
        typedef iterator self_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef std::forward_iterator_tag iterator_category;
        typedef int difference_type;
        
        iterator(Node* ptr = nullptr) : current(ptr) {
        }

        T& operator*() const {
			return current->data;
		}

		T* operator->() const {
			return &(current->data);
		}

		virtual iterator& operator++() {
		    inc();
			return *this;
		}

		virtual iterator operator++(int) {
			iterator tmp = *this;
			inc();
			return tmp;
		}
        
        virtual iterator& operator+(int n) {
		    for (int i = 0; i < n; i++) {
		        inc();
		    }
			return *this;
		}
        
        virtual iterator& operator-(int n) {
		    for (int i = 0; i < n; i++) {
		        inc();
		    }
			return *this;
		}
        
        virtual void inc() {
            if(iterator::current->right != nullptr) {
                iterator::current = iterator::current->right;
                while(iterator::current->left !=nullptr) {
                    iterator::current = iterator::current->left;
                }
            }
            else {
                while(iterator::current->parent != nullptr && iterator::current->parent->left != iterator::current) {
                    iterator::current = iterator::current->parent;
                }
                iterator::current = iterator::current->parent;
            }
        }
        
		bool operator==(const iterator& rhs) const {
			return current == rhs.current;
		}

		bool operator!=(const iterator& rhs) const {
			return current != rhs.current;
		}
        
    };
    
	class preorder_iterator : public BinaryTree<T>::iterator {
	public:    
	    preorder_iterator(Node* ptr = nullptr) : iterator(ptr) {
	        init();
        }
	    
	    void init() {
        }
        
        void inc() {
            if(iterator::current->left != nullptr) {
                iterator::current = iterator::current->left;
            }
            else if(iterator::current->right != nullptr) {
                iterator::current = iterator::current->right;
            }
            else {
                while(iterator::current->parent != nullptr && (iterator::current->parent->left != iterator::current || iterator::current->parent->right == nullptr)) {
                    iterator::current = iterator::current->parent;
                }
                if(iterator::current->parent == nullptr) {
                    iterator::current = nullptr;
                }
                else {
                    iterator::current = iterator::current->parent->right;
                }
            }
        }
	};
	
	class inorder_iterator : public BinaryTree<T>::iterator {
	public:
	    inorder_iterator(Node* ptr = nullptr) : iterator(ptr) {
	        init();
        }
        
        void init() {
            if(iterator::current != nullptr) {
                while(iterator::current->left != nullptr) {
                    iterator::current = iterator::current->left;
                }
            }
        }
        
        void inc() {
            if(iterator::current->right != nullptr) {
                iterator::current = iterator::current->right;
                while(iterator::current->left !=nullptr) {
                    iterator::current = iterator::current->left;
                }
            }
            else {
                while(iterator::current->parent != nullptr && iterator::current->parent->left != iterator::current) {
                    iterator::current = iterator::current->parent;
                }
                iterator::current = iterator::current->parent;
            }
        }
	};
	
	class postorder_iterator : public BinaryTree<T>::iterator {
	public:
	    postorder_iterator(Node* ptr = nullptr) : iterator(ptr) {
	        init();
        }
		
        void init() {
            if(iterator::current != nullptr) {
                while(iterator::current->left != nullptr || iterator::current->right != nullptr) {
                   if(iterator::current->left != nullptr) {
                       iterator::current = iterator::current->left;
                   }
                   else {
                       iterator::current = iterator::current->right;
                   }
                }
            }
        }
        
        void inc() {
            if(iterator::current->parent == nullptr) {
                iterator::current = nullptr; 
            }
            else {
                if(iterator::current->parent->right == iterator::current) {
                    iterator::current = iterator::current->parent;
                }
                else {
                    iterator::current = iterator::current->parent;
                    if(iterator::current->right != nullptr) {
                        iterator::current = iterator::current->right;
                        while(iterator::current->left != nullptr || iterator::current->right != nullptr) {
                            if(iterator::current->left != nullptr) {
                                iterator::current = iterator::current->left;
                            }
                            else {
                                iterator::current = iterator::current->right;
                            }
                        }
                    }
                }
            }
        }
        
	};
	
	inorder_iterator begin() {
		return inorder_iterator{root};
	}
	inorder_iterator end() {
		return inorder_iterator{nullptr};
	}
	preorder_iterator begin_preorder() {
	    return preorder_iterator{root};
	}
	preorder_iterator end_preorder() {
	    return preorder_iterator{nullptr};
	}
	inorder_iterator begin_inorder() {
        return inorder_iterator{root};	    
	}
	inorder_iterator end_inorder() {
	    return inorder_iterator{nullptr};
	}
	postorder_iterator begin_postorder() {
	    return postorder_iterator{root};
	}
	postorder_iterator end_postorder() {
	    return postorder_iterator{nullptr};
	}
	
	friend std::ostream& operator<<(std::ostream& os, const BinaryTree<T>& b) {
	    std::queue<Node*> q;
	    if(b.root != nullptr) {
	        q.push(b.root);
	    }
	    while(!q.empty()) {
	        Node* n = q.front();
	        q.pop();
	        if(n != nullptr) {
	            os << n->data << " ,";
	            q.push(n->left);
	            q.push(n->right);
	        }
	    }
	    return os << std::endl;
	}
	
private:
    
    Node* find(Node* n, const T& data) {
        if(n == nullptr) {
            return nullptr;
        }
        if(n->data == data) {
            return n;
        }
        Node* left = find(n->left, data);
        if(left != nullptr) {
            return left;
        }
        return find(n->right, data);
    }
    
    Node* find(const T& data) {
        return find(root, data);
    }
    
    void clean(Node* n) {
        if(n != nullptr) {
            clean(n->left);
            clean(n->right);
            delete(n);
        }
        n = nullptr;
    }
    
    void copy(Node* other_root) {
        if(other_root != nullptr) {
            root = new Node(T(other_root->data), nullptr, nullptr, nullptr);
            copy(root,other_root);
        }
    }
    
    void copy(Node* n, Node* m) {
        if(m != nullptr) {
            if(m->left != nullptr) {
                n->left = new Node(T(m->left->data), nullptr, nullptr, n);
                copy(n->left, m->left);
            }
            if(m->right != nullptr) {
                n->right = new Node(T(m->right->data), nullptr, nullptr, n);
                copy(n->right, m->right);
            }
        }
    }
    
    Node* root;
};