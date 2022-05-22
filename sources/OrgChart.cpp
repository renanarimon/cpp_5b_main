#include "OrgChart.hpp"
#include <algorithm>
#include <stack>
#include <map>
#include <queue>

namespace ariel
{
    /*------------ Node constructors & destructor ------------*/

    /*constructor*/
    OrgChart::Node::Node(std::string &data) : _data(data), _nextL(nullptr), _nextR(nullptr), _nextP(nullptr) {}

    /*destructor*/
    OrgChart::Node::~Node()
    {
        for (Node *child : this->_children)
        {
            delete child;
        }
    }

    /*copy constructor*/
    OrgChart::Node::Node(const Node &other)
        : _data(other._data), _nextL(other._nextL), _nextR(other._nextR), _nextP(other._nextP)
    {
        std::vector<Node *> v;
        for (Node *child : other._children)
        {
            v.push_back(child);
        }
        this->_children = v;
    }


    /*copy assignment operator - deep copy & assignment*/
    OrgChart::Node &OrgChart::Node::operator=(const Node &other)
    {
        if (&other != this)
        {
            this->_data = other._data;
            std::vector<Node *> v;
            for (Node *child : other._children)
            {
                v.push_back(child);
            }
            this->_children = v;
            this->_nextL = other._nextL;
            this->_nextR = other._nextR;
            this->_nextP = other._nextP;
        }
        return *this;
    }

    /*move constructor - copy constructor & delete other*/
    OrgChart::Node::Node(Node &&other) noexcept
        : _data(std::move(other._data)),
          _children(std::move(other._children)),
          _nextL(other._nextL),
          _nextR(other._nextR),
          _nextP(other._nextP) {}

    /*move assignment operator - copy & assign & delete other*/
    OrgChart::Node &OrgChart::Node::operator=(Node &&other) noexcept
    {
        this->_data = other._data;
        std::vector<Node *> v;
        for (Node *child : other._children)
        {
            v.push_back(child);
        }
        this->_children = v;
        this->_nextL = other._nextL;
        this->_nextR = other._nextR;
        this->_nextP = other._nextP;
        std::move(other._data);
        return *this;
    }

    /*-------- OrgChart constructors & destructor --------*/
    
    /*constructor*/
    OrgChart::OrgChart()
    {
        this->_root = nullptr;
    }

    /*destructor*/
    OrgChart::~OrgChart()
    {
        delete this->_root;
    }

    /*copy constructor*/
    OrgChart::OrgChart(OrgChart &other)
    {
        this->_root = other._root;
    }

    /*move constructor - copy constructor & delete other*/
    OrgChart::OrgChart(OrgChart &&org) noexcept
    {
        this->_root = org._root;
        org._root = nullptr;
    }

    /*copy assignment operator - deep copy & assignment*/
    OrgChart &OrgChart::operator=(const OrgChart &org)
    {
        if (&org != this)
        {
            this->_root = org._root;
        }

        return *this;
    }

    /*move assignment operator - copy & assign & delete other*/
    OrgChart &OrgChart::operator=(OrgChart &&org) noexcept
    {
        this->_root = org._root;
        org._root = nullptr;
        return *this;
    }

    /*------------ OrgChart Methods ------------*/
    
    /* add root:
        if root not exist -> create new
        else -> change name
        @return: OrgChart&
    */
    OrgChart &OrgChart::add_root(std::string name)
    {
        if (this->_root != nullptr)
        {
            this->_root->_data = name;
        }
        else
        {
            this->_root = new Node(name);
        }
        return *this;
    }

    /*add sub:
    @employer: node in tree
    @employee: new node
    if employer exist -> add employee as his child
    else -> throw exception*/
    OrgChart &OrgChart::add_sub(const std::string &employer, std::string employee)
    {
        OrgChart::Node *boss = find(employer);
        if (boss == nullptr)
        {
            throw std::invalid_argument("employer doesn't exist");
        }
        Node *n = new Node(employee);
        boss->_children.push_back(n);
        return *this;
    }

    /*------------ begin & end iterator in OrgChart ------------*/

    /*  begin: arranges the pointers & return new iterator to first node
        end: return new iterator to after last node*/

    OrgChart::iterator OrgChart::begin_level_order()
    {
        if (this->_root == nullptr){throw std::invalid_argument("orgChart is empty");}
        return OrgChart::iterator("level", this->_root);
    }
    OrgChart::iterator OrgChart::end_level_order()
    {
        if (this->_root == nullptr){throw std::invalid_argument("orgChart is empty");}
        return OrgChart::iterator("end", nullptr);
    }
    OrgChart::iterator OrgChart::begin_reverse_order()
    {
        if (this->_root == nullptr){throw std::invalid_argument("orgChart is empty");}
        return OrgChart::iterator("reverse", this->_root);
    }
    OrgChart::iterator OrgChart::reverse_order()
    {
        if (this->_root == nullptr){throw std::invalid_argument("orgChart is empty");}
        return OrgChart::iterator("end", nullptr);
    }
    OrgChart::iterator OrgChart::begin_preorder()
    {
        if (this->_root == nullptr){throw std::invalid_argument("orgChart is empty");}
        return OrgChart::iterator("preOrder", this->_root);
    }
    OrgChart::iterator OrgChart::end_preorder()
    {
        if (this->_root == nullptr){throw std::invalid_argument("orgChart is empty");}
        return OrgChart::iterator("end", nullptr);
    }
    OrgChart::iterator OrgChart::begin()
    {
        if (this->_root == nullptr){throw std::invalid_argument("orgChart is empty");}
        return begin_level_order();
    }
    OrgChart::iterator OrgChart::end()
    {
        if (this->_root == nullptr){throw std::invalid_argument("orgChart is empty");}
        return end_level_order();
    }

    /*cout operator*/
    std::ostream &operator<<(std::ostream &out, const OrgChart &org)
    {
        std::deque<OrgChart::Node *> q;
        if (org._root == nullptr)
        {
            throw std::out_of_range("OrgChart is empty");
        }
        q.push_back(org._root);
        while (!q.empty())
        {
            size_t len = q.size();
            for (size_t i = 0; i < len; i++)
            {
                OrgChart::Node *tmp = q.front();
                out << tmp->_data << "  ";
                if (!tmp->_children.empty())
                {
                    for (OrgChart::Node *child : tmp->_children)
                    {
                        q.push_back(child);
                    }
                }
                q.pop_front();
            }
            out << "\n";
        }

        return out;
    }

    /* find the first accuracy of this name
        @return: Node* */
    OrgChart::Node *OrgChart::find(const std::string &s)
    {
        if (this->_root == nullptr)
        {
            return nullptr;
        }
        std::deque<Node *> q;
        q.push_back(this->_root);

        while (!q.empty())
        {
            Node *tmp = q.front();
            if (tmp->_data == s)
            {
                return tmp;
            }
            for (Node *child : tmp->_children)
            {
                q.push_back(child);
            }
            q.pop_front();
        }
        return nullptr;
    }

    /*BFS - level order*/
    void OrgChart::iterator::BFS(Node *root)
    {
        std::deque<Node *> q;
        if (root == nullptr)
        {
            throw std::out_of_range("OrgChart is empty");
        }

        q.push_back(root);
        Node *tmp = nullptr;
        while (!q.empty())
        {
            tmp = q.front();
            q.pop_front();
            for (Node *child : tmp->_children)
            {
                q.push_back(child);
            }
            tmp->_nextL = q.front();
        }
        if (tmp != nullptr)
        {
            tmp->_nextL = nullptr;
        }
    }

    /*Reverse_BFS - reverse level order*/
    void OrgChart::iterator::Reverse_BFS(Node *root)
    {
        std::deque<Node *> q;
        if (root == nullptr)
        {
            throw std::out_of_range("OrgChart is empty");
        }

        q.push_back(root);
        Node *tmp = nullptr;
        while (!q.empty())
        {
            tmp = q.front();
            q.pop_front();
            for (int i = (int)tmp->_children.size() - 1; i >= 0; i--)
            {
                q.push_back(tmp->_children.at((size_t)i));
            }
            tmp->_nextR = q.front();
        }
        if (tmp != nullptr)
        {
            tmp->_nextR = nullptr;
        }

        reverse(root);
    }

    /* reverse linked list - help func to 'Reverse_BFS' */
    void OrgChart::iterator::reverse(Node *root)
    {
        // Initialize current, previous and next pointers
        Node *current = root;
        Node *prev = nullptr;
        Node *next = nullptr;

        while (current != nullptr)
        {
            // Store next
            next = current->_nextR;
            // Reverse current node's pointer
            current->_nextR = prev;
            // Move pointers one position ahead.
            prev = current;
            current = next;
        }
        _curr = prev; // iterator _curr point to prev
    }

    /* PreOrder */
    void OrgChart::iterator::PreOrder(Node *root)
    {
        if (root == nullptr){throw std::out_of_range("OrgChart is empty");}
        std::stack<Node *> st;
        st.push(root);
        Node *tmp = nullptr;
        while (!st.empty())
        {
            tmp = st.top();
            st.pop();
            for (int i = (int)tmp->_children.size() - 1; i >= 0; i--)
            {
                st.push(tmp->_children.at((size_t)i));
            }
            if (!st.empty())
            {
                tmp->_nextP = st.top();
            }
        }
        if (tmp != nullptr)
        {
            tmp->_nextP = nullptr;
        }
    }

    /*-------- iterator constructors & destructor --------*/

    /*constructor - arrange nodes by flag*/
    OrgChart::iterator::iterator(const std::string &flag, OrgChart::Node *root)
        : _flag(flag), _curr(root)
    {
        if (flag == "level")
        {
            BFS(root);
        }
        else if (flag == "reverse")
        {
            Reverse_BFS(root);
        }
        else if (flag == "preOrder")
        {
            PreOrder(root);
        }
    }

    /*destructor*/
    OrgChart::iterator::~iterator()
    {
        this->_curr = nullptr;
        std::move(this->_flag);
    }

    /*copy constructor*/
    OrgChart::iterator::iterator(iterator &other)
    {
        this->_curr = other._curr;
        this->_flag = other._flag;
    }

    /*move constructor - copy constructor & delete other*/
    OrgChart::iterator::iterator(iterator &&other) noexcept
    {
        this->_curr = other._curr;
        this->_flag = other._flag;
        other._curr = nullptr;
    }

    /*copy assignment operator - deep copy & assignment*/
    OrgChart::iterator &OrgChart::iterator::operator=(const iterator &other)
    {
        if (&other != this)
        {
            this->_curr = other._curr;
            this->_flag = other._flag;
        }

        return *this;
    }

    /*move assignment operator - copy & assign & delete other*/
    OrgChart::iterator &OrgChart::iterator::operator=(iterator &&other) noexcept
    { 
        this->_curr = other._curr;
        this->_flag = other._flag;
        other._curr = nullptr;
        return *this;
    }

    /*------------ iterator operators ------------*/
    

    std::string &OrgChart::iterator::operator*() const
    {
        return _curr->_data;
    }

    std::string *OrgChart::iterator::operator->() const
    {
        return &(_curr->_data);
    }

    /* increament iterator by flag (iter++) */
    OrgChart::iterator &OrgChart::iterator::operator++()
    {
        if (_flag == "level")
        {
            _curr = _curr->_nextL;
        }
        else if (_flag == "reverse")
        {
            _curr = _curr->_nextR;
        }
        else if (_flag == "preOrder")
        {
            _curr = _curr->_nextP;
        }
        return *this;
    }

    /* increament iterator by flag (++iter) */
    OrgChart::iterator OrgChart::iterator::operator++(int)
    {
        iterator tmp = *this;
        (*this)++;
        return tmp;
    }

    bool OrgChart::iterator::operator==(const OrgChart::iterator &other)
    {
        return this->_curr == other._curr;
    }
    
    bool OrgChart::iterator::operator!=(const OrgChart::iterator &other)
    {
        return this->_curr != other._curr;
    }

}