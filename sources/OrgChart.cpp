#include "OrgChart.hpp"
#include <algorithm>
#include <stack>
#include <map>
#include <queue>

namespace ariel
{
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

    OrgChart::Node::~Node(){
        for(Node* child : this->_children){
            delete child;
        }
    }

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
    OrgChart::Node::Node(Node &&other) noexcept
        : _data(std::move(other._data)),
          _children(std::move(other._children)),
          _nextL(other._nextL),
          _nextR(other._nextR),
          _nextP(other._nextP) {}

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

    OrgChart::OrgChart()
    {
        std::cout << "constructor OrgChart" << std::endl;
        this->_root = nullptr;
    }
    OrgChart::OrgChart(OrgChart &other)
    {
        std::cout << "copy constructor OrgChart" << std::endl;
        this->_root = other._root;
    }
    OrgChart::OrgChart(OrgChart &&org) noexcept
    {
        std::cout << "OrgChart(OrgChart &&org) noexcept" << std::endl;
        this->_root = org._root;
        org._root = nullptr;
    }
    OrgChart &OrgChart::operator=(const OrgChart &org)
    {
        if(&org != this){
            this->_root = org._root;
        }
        
        return *this;
    }
    OrgChart &OrgChart::operator=(OrgChart &&org) noexcept
    {
        std::cout << "&OrgChart::operator=(OrgChart &&org)" << std::endl;
        this->_root = org._root;
        org._root = nullptr;
        return *this;
    }
    OrgChart::~OrgChart()
    {
        std::cout << "~OrgChart()" << std::endl;
        delete this->_root;
    }
    OrgChart &OrgChart::add_root(std::string name)
    {
        std::cout << "add_root: " << name << ", " << this << std::endl;
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

    OrgChart &OrgChart::add_sub(const std::string& employer, std::string employee)
    {
        OrgChart::Node *boss = find(employer);
        if (boss == nullptr)
        {
            throw std::invalid_argument("employer doesn't exist");
        }
        std::cout << "add_sub: " << employee << " to: " << *this << std::endl;
        Node *n = new Node(employee);
        if (n->_children.empty())
        {
        }
        boss->_children.push_back(n);
        return *this;
    }

    OrgChart::iterator OrgChart::begin_level_order()
    {
        if (this->_root == nullptr)
        {
            throw std::invalid_argument("orgChart is empty");
        }
        return OrgChart::iterator("level", this->_root);
    }
    OrgChart::iterator OrgChart::end_level_order()
    {
        if (this->_root == nullptr)
        {
            throw std::invalid_argument("orgChart is empty");
        }
        return OrgChart::iterator("end", nullptr);
    }
    OrgChart::iterator OrgChart::begin_reverse_order()
    {
        if (this->_root == nullptr)
        {
            throw std::invalid_argument("orgChart is empty");
        }
        return OrgChart::iterator("reverse", this->_root);
    }
    OrgChart::iterator OrgChart::reverse_order()
    {
        if (this->_root == nullptr)
        {
            throw std::invalid_argument("orgChart is empty");
        }
        return OrgChart::iterator("end", nullptr);
    }
    OrgChart::iterator OrgChart::begin_preorder()
    {
        if (this->_root == nullptr)
        {
            throw std::invalid_argument("orgChart is empty");
        }
        return OrgChart::iterator("preOrder", this->_root);
    }
    OrgChart::iterator OrgChart::end_preorder()
    {
        if (this->_root == nullptr)
        {
            throw std::invalid_argument("orgChart is empty");
        }
        return OrgChart::iterator("end", nullptr);
    }
    OrgChart::iterator OrgChart::begin()
    {
        if (this->_root == nullptr)
        {
            throw std::invalid_argument("orgChart is empty");
        }
        return begin_level_order();
    }
    OrgChart::iterator OrgChart::end()
    {
        if (this->_root == nullptr)
        {
            throw std::invalid_argument("orgChart is empty");
        }
        return end_level_order();
    }

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

    OrgChart::Node *OrgChart::find(const std::string& s)
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
        if(tmp != nullptr){
            tmp->_nextL = nullptr;
        }
        
    }

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
            for (int i =(int) tmp->_children.size() - 1; i >= 0; i--)
            {
                q.push_back(tmp->_children.at((size_t)i));
            }
            tmp->_nextR = q.front();
        }
        if(tmp != nullptr){
            tmp->_nextR = nullptr;
        }
        
        reverse(root);
    }
    void OrgChart::iterator::reverse(Node *root)
    {
        // Initialize current, previous and next pointers
        Node *current = root;
        Node *prev = nullptr;
        Node* next = nullptr;

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

        _curr = prev;
    }

    void OrgChart::iterator::PreOrder(Node *root)
    {
        if (root == nullptr)
        {
            throw std::out_of_range("OrgChart is empty");
        }
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
        if(tmp != nullptr){
            tmp->_nextP = nullptr;
        }
        
    }

    /*iterator*/
    OrgChart::iterator::iterator(const std::string& flag, OrgChart::Node *root)
        : _flag(flag), _curr(root)
    {
        std::cout << "consructor iter: " << flag << ", root: " << root << std::endl;

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

    OrgChart::iterator::~iterator()
    {
        this->_curr = nullptr;
        std::move(this->_flag);
        std::cout << "~iterator()" << std::endl;
    }
    std::string &OrgChart::iterator::operator*() const
    {
        return _curr->_data;
    }
    std::string *OrgChart::iterator::operator->() const
    {
        return &(_curr->_data);
    }
    OrgChart::iterator::iterator(iterator &other)
    {
        std::cout << "copy iterator" << std::endl;
        this->_curr = other._curr;
        this->_flag = other._flag;
    }
    OrgChart::iterator::iterator(iterator &&other) noexcept
    {
        std::cout << "iterator(iterator &&other)" << std::endl;
        this->_curr = other._curr;
        this->_flag = other._flag;
        other._curr = nullptr;
    }
    OrgChart::iterator &OrgChart::iterator::operator=(const iterator &other)
    {
        if(&other != this){
            this->_curr = other._curr;
            this->_flag = other._flag;
        }
        
        return *this;
    }
    OrgChart::iterator &OrgChart::iterator::operator=(iterator &&other) noexcept
    { // TODO
        std::cout << "operator=(iterator&& other)" << std::endl;
        this->_curr = other._curr;
        this->_flag = other._flag;
        other._curr = nullptr;
        return *this;
    }

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