#pragma once
#include <stack>
#include <unordered_set>

namespace ariel{

    template <typename T>
    class BinaryTree
    {
        private:

        typedef struct Node{
            T node_data;
            Node *left;
            Node *right;
            Node(const T& v, Node *leftN = nullptr, Node *rightN = nullptr) : left(leftN), right(rightN), node_data(v){}
        }Node;

        Node* root;

        //find a node in a tree using inorder
        Node* findNode(T value)
        {
            if(root != nullptr)
            {
                for (auto it = begin_inorder(); it != end_inorder(); ++it)
                {
                    if (*it == value)
                    {
                        return it.getNode();
                    }
                }
            }
            return nullptr;
        }

        //deleting all nodes in a tree
        void removeTree()
        {
            if(!root)
            {
                for (auto it = begin_inorder(); it != end_inorder();++it){
                    Node *temp = it.getNode();
                    delete temp;
                }
            }
        }

        void deepCopy(Node* node, Node* otherTreeNode)
        {
            if(otherTreeNode->right)
            {
                node->right = new Node(otherTreeNode->right->node_data);
                deepCopy(node->right, otherTreeNode->right);
            }
            if(otherTreeNode->left)
            {
                node->left = new Node(otherTreeNode->left->node_data);
                deepCopy(node->left, otherTreeNode->left);
            }
        }

        void copy(const BinaryTree& otherTree)
        {
            if(otherTree.root)
            {
                root = new Node(otherTree.root->node_data);
                deepCopy(root, otherTree.root);
            }
        }

    public:
        BinaryTree(){
            root = nullptr;
        }
        
        ~BinaryTree(){
            removeTree();
        }
        
        BinaryTree(const BinaryTree& otherTree){   
            removeTree();
            copy(otherTree);
        }

        BinaryTree(BinaryTree&& otherTree)noexcept{
            root = otherTree.root;
            otherTree.root = nullptr;
        }

        BinaryTree& operator=(const BinaryTree& otherTree){
            if(this != &otherTree){
                removeTree();
                copy(otherTree);
            }
            return *this;
        }

        BinaryTree &operator=(BinaryTree&& otherTree) noexcept{
            root = otherTree.root;
            otherTree.root = nullptr;
            return *this;
        }

        BinaryTree& add_root(T value)
        {
            if(root == nullptr)
            {
                root = new Node(value);
            }
            else
            {
                root->node_data = value;
            }
            return *this;
        }

        BinaryTree& add_left(T parent, T left_child)
        {
            Node* parentNode = findNode(parent);
            if(parentNode != nullptr)
            {
                if(parentNode->left == nullptr)
                {
                    parentNode->left = new Node(left_child);
                }
                else
                {
                    (parentNode->left)->node_data = left_child;
                }
            }
            else
            {
                throw std::invalid_argument("Parent not found");
            }
            return *this;
        }

        BinaryTree& add_right(T parent, T right_child)
        {
            Node* parentNode = findNode(parent);
            if(parentNode != nullptr)
            {
                if(parentNode->right == nullptr)
                {
                    parentNode->right = new Node(right_child);
                }
                else
                {
                    (parentNode->right)->node_data = right_child;
                }
            }
            else
            {
                throw std::invalid_argument("Parent not found");
            }
            return *this;
        }
        
        friend std::ostream& operator<<(std::ostream &out, const ariel::BinaryTree<T>& binaryTree){
            for(auto it : binaryTree){
                out << it << " ";
            }
            return out;
        }
    
//============================POSTORDER==================================================================================================================
        
        class preorder_iterator
        {
            
            private:
                Node* currNodePtr;
                std::stack<Node*> nodeStack;

            public:
                preorder_iterator(Node* ptr = nullptr)
                {
                    if(ptr != nullptr)
                    {
                        nodeStack.push(ptr);
                        currNodePtr = nodeStack.top();
                    }
                    else
                    {
                        currNodePtr = nullptr;
                    }
                }

		        T& operator*() const {
		        	return currNodePtr->node_data;
		        }

		        T* operator->() const {
		        	return &(currNodePtr->node_data);
		        }

		        preorder_iterator &operator++()
                {
                    if (!nodeStack.empty())
                    {
                        nodeStack.pop();
                        if (currNodePtr->right !=nullptr)
                        {
                            nodeStack.push(currNodePtr->right);
                        }
                        if (currNodePtr->left !=nullptr)
                        {
                            nodeStack.push(currNodePtr->left);
                        }
                        if (!nodeStack.empty())
                        {
                            currNodePtr = nodeStack.top();
                            return *this;
                        }
                    }
                    currNodePtr =nullptr;
                    return *this;
                }

		        preorder_iterator operator++(int)
                {
		        	preorder_iterator tmp= *this;
		        	operator++();
		        	return tmp;
		        }

		        bool operator==(const preorder_iterator& rhs) const {
		        	return (currNodePtr == rhs.currNodePtr);
		        }

		        bool operator!=(const preorder_iterator& rhs) const {
		        	return (currNodePtr != rhs.currNodePtr);
		        }

        }; // end of preorder_iterator class

//=============================INORDER==========================================================================================

        class inorder_iterator
        {
            private:
                Node* currNodePtr;
                std::stack<Node*> nodeStack;

            public:
                inorder_iterator(Node* ptr = nullptr) 
                {
                    while(ptr != nullptr)
                    {
                        nodeStack.push(ptr);
                        ptr = ptr->left;
                    }
                    if(nodeStack.empty())
                    {
                        currNodePtr = nullptr;
                    }
                    else
                    {
                        currNodePtr = nodeStack.top();
                        nodeStack.pop();
                    }
                }

		        T& operator*() const {
		        	return currNodePtr->node_data;
		        }

		        T* operator->() const {
		        	return &(currNodePtr->node_data);
		        }

		        inorder_iterator &operator++()
                {
                    if (!nodeStack.empty() || currNodePtr != nullptr)
                    {
                        if (currNodePtr->right != nullptr)
                        {
                            Node* temp =currNodePtr->right;
                            nodeStack.push(temp);
                            while (temp->left != nullptr)
                            {
                                temp =temp->left;
                                nodeStack.push(temp);
                            }
                            currNodePtr =nodeStack.top();
                            nodeStack.pop();
                            return *this;
                        }
                        if (nodeStack.empty())
                        {
                            currNodePtr = nullptr;
                            return *this;
                        }
                        currNodePtr =nodeStack.top();
                        nodeStack.pop();
                        return *this;
                        
                    }
                    
                    currNodePtr =nullptr;
                    return *this;
                    

                }

		        inorder_iterator operator++(int) {
		        	inorder_iterator tmp= *this;
                    operator++();
		        	return tmp;
		        }

		        bool operator==(const inorder_iterator& rhs) const {
		        	return (currNodePtr == rhs.currNodePtr);
		        }

		        bool operator!=(const inorder_iterator& rhs) const {
		        	return (currNodePtr != rhs.currNodePtr);
		        }

                Node* getNode(){
                    return currNodePtr;
                }
                
            
        }; // end of inorder_iterator class

//=============================POSTORDER==========================================================================================
 
        class postorder_iterator
        {
            private:
                Node* currNodePtr;
                Node* root;
                std::unordered_set<Node*> visited;

            public:
                postorder_iterator(Node* ptr = nullptr)
                {
                    if(ptr == nullptr)
                    {
                        currNodePtr = nullptr;
                    }
                    else
                    {
                        root = ptr;
                        while (ptr !=nullptr && visited.find(ptr) == visited.end()) 
                        {
                            if (ptr->left != nullptr && visited.find(ptr->left) == visited.end())
                            {
                                ptr = ptr->left;
                            }
                            else
                            {
                                if (ptr->right && visited.find(ptr->right) == visited.end())
                                {
                                    ptr = ptr->right;
                                }
                                else 
                                {
                                    currNodePtr = ptr;
                                    visited.insert(ptr);
                                    break;
                                }
                            }
                        }
                    }
                }

		        T& operator*() const {
		        	return currNodePtr->node_data;
		        }

		        T* operator->() const {
		        	return &(currNodePtr->node_data);
		        }

		        postorder_iterator& operator++() 
                {
                        Node* ptr = root;

                            while (ptr != nullptr && visited.find(ptr) == visited.end()) 
                            {
                                if (ptr->left != nullptr && visited.find(ptr->left) == visited.end())
                                {
                                    ptr = ptr->left;
                                }
                                
                                else
                                {
                                    if (ptr->right && visited.find(ptr->right) == visited.end())
                                    {
                                        ptr = ptr->right;
                                    }
                                    else 
                                    {
                                        currNodePtr = ptr;
                                        visited.insert(ptr);
                                        return *this;   
                                    }
                                }
                            }
                        
                        currNodePtr = nullptr;
                        return *this;
                    }

		        postorder_iterator operator++(int) {
		        	postorder_iterator tmp= *this;
                    operator++();
		        	return tmp;
		        }

		        bool operator==(const postorder_iterator& rhs) const {
		        	return (currNodePtr == rhs.currNodePtr);
		        }

		        bool operator!=(const postorder_iterator& rhs) const {
                    if(currNodePtr == nullptr && rhs.currNodePtr == nullptr){
                   }
		        	return (currNodePtr != rhs.currNodePtr);
		        }
                
            
        }; // end of postorder_iterator class

        preorder_iterator begin_preorder() const{
            return preorder_iterator{root};
        }
        preorder_iterator end_preorder() const{
            return preorder_iterator{};
        }
        inorder_iterator begin_inorder() const{
            return inorder_iterator{root};
        }
        inorder_iterator end_inorder() const{
            return inorder_iterator{nullptr};
        }
        inorder_iterator begin() const{
            return inorder_iterator{root};
        }
        inorder_iterator end() const{
            return inorder_iterator{};
        }
        postorder_iterator begin_postorder() const{
            return postorder_iterator{root};
        }
        postorder_iterator end_postorder() const{
            return postorder_iterator{nullptr};
        }

    };//end of BinaryTree class
}; // end of namespace