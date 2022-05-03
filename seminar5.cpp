template <class T>
struct DefaultComparator
{
    int operator () (const T& l, const T& r) const 
    {
        if ( l < r ) return -1;
        if ( l > r ) return 1;
        return 0;
    }
};

template <class Key, class Value, class Comparator = DefaultComparator<Key> >
class AVLTree 
{
    struct Node 
    {
        Key key;
        Value value;
        
        uint8_t height;
        
        Node* left;
        Node* right;
        
        Node( const Key& key, const Value& value ) :
            key( key ),
            value( value ),
            height( 1 ),
            left( nullptr ),
            right( nullptr )
        {
        }
    };
public:
    AVLTree( Comparator comp = Comparator() ) : root( nullptr ), tree_size( 0 ), comp( comp )
    {
    }
    
    AVLTree( const AVLTree& ) = delete;
    AVLTree( AVLTree&& ) = delete;
    AVLTree& operator =( const AVLTree& ) = delete;
    AVLTree& operator =( AVLTree&& ) = delete;
    
    ~AVLTree();
    
    Value* find( const Key& key )
    {
        return _find( root, key );
    }
    const Value* find( const Key& key ) const
    {
        return find(key);
    }
    
    void insert( const Key& key, const Value& value )
    {
        root = _insert(root, key, value);
    }
    
    void erase( const Key& key )
    {
        root = _erase( root, key );
    }
    
private:
    Value* _find( Node* node, const Key& key)
    {
        if ( !node )
        {
            return nullptr;
        }
        int comp_res  = comp(key, node->key);
        if (comp_res == -1) 
        {
            return _find(node->left, key);
        } 
        else if (comp_res == 1) 
        {
            return _find(node->right, key);
        }
        return node;
    }
    
    Node* _insert( Node* node, const Key& key, const Value& value)
    {
        if ( !node ) 
        {
            tree_size++;
            return new Node(key, value);
        }
        
        int comp_res  = comp(key, node->key);
        if (comp_res == -1) 
        {
            node->left = _insert(node->left, key, value);
        }
        else 
        {
            node->right = _insert(node->right, key, value);
        }
        return balance(node);
    }
    
    Node* _erase( Node* node, const Key& key )
    {
        if ( !node )
        {
            return nullptr;
        }
        
        int comp_res = comp(key, node->key);
        if ( comp_res == -1 )
        {
            node->left = _erase(node->left, key);
        }
        else if ( comp_res == 1 )
        {
            node->right = _erase(node->right, key);
        }
        else
        {
            Node* left = node->left;
            Node* right = node->right;
            
            delete node;
            
            if ( !right )
            {
                return left;
            }
            
            Node* min_node = find_min(right);
            min_node->right = remove_min(right);
            min_node->left = left;
            
            return balance(min_node);
        }
        return balance( node );
    }
    
    Node* find_min( Node* node )
    {
        if ( !node->left )
        {
            return node;
        }
        return find_min(node->left);
    }
    
    Node* remove_min( Node* node )
    {
        if ( !node->left )
        {
            return node->right;
        }
        node->left = remove_min( node->left );
        return balance( node );
    }
    
    uint8_t height( Node* node ) const
    {
        return node ? node->height : 0;
    }
    
    void fix_height( Node* node )
    {
        node->height = std::max(height(node->left), height(node->right)) + 1;
    }
    
    int bfactor( Node* node ) const
    {
        return height(node->right) - height(node->left);
    }
    
    Node* rotate_left( Node* node );
    Node* rotate_right( Node* node );
    
    Node* balance( Node* node )
    {
        fix_height(node);
        int bf = bfactor( node );
        
        if ( bf == 2 )
        {
            if ( bfactor( node->right ) < 0 )
            {
                node->right  = rotate_right( node->right );
            }
            return rotate_left( node );
        }
        else if ( bf == -2 )
        {
            if ( bfactor( node->left ) > 0 )
            {
                node->left  = rotate_left( node->left );
            }
            return rotate_right( node );
        }
        return node;
    }
    
    Node* root;
    size_t tree_size;
    Comparator comp;
};


template <typename Key>
class BTree
{
public:
    struct Node
    {
        Node(bool leaf)
        : leaf(leaf)
        {
        }
 
        ~Node()
        {
            for (Node* child: children)
            {
                delete child;
            }
        }
 
        bool leaf;
        std::vector<Key> keys;
        std::vector<Node*> children;
    };
 
    BTree(size_t min_degree)
    : root(nullptr), t(min_degree)
    {
        assert(min_degree >= 2);
    }
 
    ~BTree()
    {
        if (root)
            delete root;
    }
 
    void insert(const Key &key)
    {
        if (!root)
            root = new Node(true);
        
        if (is_node_full(root))
        {
            Node *newRoot = new Node(false);
            newRoot->children.push_back(root);
            root = newRoot;
            split(root, 0);
        }
 
        insert_non_full(root, key);
    }
 
    bool find(const Key &key)
    {
        return find_aux(root, key);
    }
 
private:
    bool is_node_full(Node *node)
    {
        return node->keys.size() == 2*t - 1;
    }
 
    bool find_aux(Node* node, const Key &key)
    {
        int i = 0;
        while (i < node->keys.size() && key > node->keys[i])
            i++;
 
        if (i < node->keys.size() && key == node->keys[i])
            return true;
        else if (node->leaf)
            return false;
        else
            return find_aux(node->children[i], key);
    }
 
    void split(Node *node, size_t index);
 
 
    void insert_non_full(Node *node, const Key &key)
    {
        int pos = node->keys.size() - 1;
        if (node->leaf)
        {
            node->keys.resize(node->keys.size() + 1);
            while (pos >= 0 && key < node->keys[pos])
            {
                node->keys[pos + 1] = node->keys[pos];
                pos--;
            }
            node->keys[pos + 1] = key;
        }
        else
        {
            while (pos >= 0 && key < node->keys[pos])
            {
                pos--;
            }
 
            if (is_node_full(node->children[pos + 1]))
            {
                split(node, pos + 1);
                if (key > node->keys[pos + 1])
                {
                    pos++;
                }
            }
            insert_non_full(node->children[pos + 1], key);
        }
    }
 
    
    
 
    Node *root;
    size_t t; // minimum degree
  
};
