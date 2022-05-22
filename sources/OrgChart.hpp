#pragma once
#include <iostream>
#include <vector>
#include <deque>
namespace ariel
{
    class OrgChart
    {

        /* Node in OrgChart*/
        struct Node
        {
            std::string _data;
            std::vector<Node *> _children;
            Node *_nextL;                           // pointer to next node in level order
            Node *_nextR;                           // pointer to next node in reverse order
            Node *_nextP;                           // pointer to next node in pre order
            Node(std::string &data);                // constructor
            ~Node();                                // destructor
            Node(const Node &other);                // copy constructor
            Node &operator=(const Node &other);     // copy assignment operator
            Node(Node &&other) noexcept;            // move constructor
            Node &operator=(Node &&other) noexcept; // move assignment operator

        }; // end Node;

    public:
        /* iterator:
            3 possible orders to travel tree:
                1. level order
                2. reverse level order
                3. preOrder
        */
        class iterator
        {
        private:
            Node *_curr;                      // curr node
            std::string _flag;                // curr order
            static void BFS(Node *root);      // Arranges the pointers in level order
            void Reverse_BFS(Node *root);     // Arranges the pointers in reverse order
            static void PreOrder(Node *root); // Arranges the pointers in pre order
            void reverse(Node *root);         // help func - reverse linkedlist

        public:
            iterator(const std::string &flag, Node *root);  // constructor
            ~iterator();                                    // destructor
            iterator(iterator &other);                      // copy constructor
            iterator(iterator &&other) noexcept;            // move constructor
            iterator &operator=(const iterator &other);     // copy assignment operator
            iterator &operator=(iterator &&other) noexcept; // move assignment operator
            std::string &operator*() const;                 // return ptr to curr node
            std::string *operator->() const;                // return ref to curr node
            iterator &operator++();                         // iter++
            iterator operator++(int);                       // ++iter
            bool operator==(const iterator &other);
            bool operator!=(const iterator &other);
        }; // end iterator;

        /*back to OrgChart*/
    private:
        Node *_root;                                                             // chart root
        friend std::ostream &operator<<(std::ostream &out, const OrgChart &org); // cout operator

    public:
        OrgChart();                                                           // constructor
        OrgChart(OrgChart &other);                                            // copy constructor
        OrgChart(OrgChart &&org) noexcept;                                    // move constructor
        OrgChart &operator=(const OrgChart &org);                             // copy assignment operator
        OrgChart &operator=(OrgChart &&org) noexcept;                         // move assignment operator
        ~OrgChart();                                                          // destructor
        OrgChart &add_root(std::string name);                                 // add root
        OrgChart &add_sub(const std::string &employer, std::string employee); // add sub

        // return iterator to of specific order
        iterator begin_level_order();
        iterator end_level_order();
        iterator begin_reverse_order();
        iterator reverse_order();
        iterator begin_preorder();
        iterator end_preorder();
        iterator begin(); // same as level order
        iterator end();   // same as level order

        OrgChart::Node *find(const std::string &s); // find Node in OrgChart by name
    };

}
