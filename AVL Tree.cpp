#include<iostream>
#include<cstdlib>
#include<sstream>
#include<iomanip>
#include<math.h>
#include<queue>
#include<vector>

using namespace std;
bool DEBUG = false;

class Node
{
	private:
		int key;
		Node* left;
		Node* right;
		Node* parent;
	public:
		Node(int key): key(key),left(nullptr),right(nullptr), parent(nullptr) {}
		int getKey() { return this->key;}
		friend class AVL;
};
//=============================================================================
class AVL
{
	private:
		Node *root;
	public:
		AVL();									//constructor
		~AVL();									//destructor
		Node* getRoot();						//returns the root of the Tree
		void insert(Node* ptr,int key);			//Insert key into tree/subtree with root ptr
		int height(Node *ptr);				    //Find the height of a tree/subtree with root ptr
		Node* find(Node *ptr,int key);		//Find and returns the node with key
		Node* findMin(Node *ptr);				//Find and return the Node with minimum key value from a tree/subtree with root ptr
		Node* findMax(Node *ptr);				//Find and return the Node with Max key value from a tree/subtree with root ptr		
		void remove(Node *ptr,int key);			//Remove a node with key from the tree/subtree with root
		void printInorder(Node* ptr);			//Traverse (inroder) and print the key of a tree/subtree with root ptr
		void printTree();						//Print 2D Tree
	private:
			void rotateLeft(Node *z);
			void rotateRight(Node *z);
			int balanceFactor(Node *ptr);
			void rebalance(Node *ptr);    			// rebalance a node

	
};

//==========================================================
void listCommands()
{
			
	cout<<"________________________________________________________"<<endl;
	cout<<"display            :Display the AVL Tree"<<endl
		<<"height             :Find the hieght of the Tree"<<endl
		<<"min                :Find the node with minimum key in AVL"<<endl
		<<"max                :Find the node with maximum key in AVL"<<endl
		<<"find <key>         :Find a node with a given key value in AVL"<<endl
		<<"insert <key>       :Insert a new node in AVL"<<endl
		<<"remove <key>       :Remove the node from AVL "<<endl
		<<"inorder            :Print the AVL in Inorder"<<endl
		<<"help               :Display the available commands"<<endl
		<<"exit               :Exit the program"<<endl;
}
//==========================================================
void demo( AVL &myAVL)
{
	myAVL.insert(myAVL.getRoot(),10);
	myAVL.insert(myAVL.getRoot(),5);
	myAVL.insert(myAVL.getRoot(),4);
	myAVL.insert(myAVL.getRoot(),7);
	myAVL.insert(myAVL.getRoot(),13);
	myAVL.insert(myAVL.getRoot(),15);
	myAVL.insert(myAVL.getRoot(),12);
	myAVL.printTree();
}
//==========================================================
int main(void)
{
	AVL myAVL;
	listCommands();
	string user_input;
	string command;
	string key;

	while(true)
	{
		try
		{
			cout<<">";
			getline(cin,user_input);

			// parse userinput into command and parameter(s)
			stringstream sstr(user_input);
			getline(sstr,command,' ');
			getline(sstr,key);
			
			Node* root = myAVL.getRoot();	//get the root of the tree
			if( user_input =="display")	myAVL.printTree();
			else if( command =="height")	cout<<"Height = "<<myAVL.height(root)<<endl;
			else if( command =="min")	
			{
				Node* min = myAVL.findMin(root);
				cout<<"Min. key = "<<((min!=nullptr)? to_string(min->getKey()):" does not exist")<<endl;
			}
			else if( command =="max")	
			{
				Node* max = myAVL.findMax(root);
				cout<<"Max. key = "<<((max!=nullptr)? to_string(max->getKey()):" does not exist")<<endl;
			}
			else if( command =="find" or command =="f")	cout<<key<<(myAVL.find(root,stoi(key))==nullptr ? " not found":" found")<<endl;
			else if( command =="insert" or command=="i")	{ myAVL.insert(root, stoi(key)); myAVL.printTree();} 
			else if( command =="remove" or command=="r")	{ myAVL.remove(root,stoi(key)); myAVL.printTree();}
			else if( command =="inorder")	{myAVL.printInorder(root); cout<<endl;}
			else if( command == "demo") demo(myAVL); 
			else if( command == "help") listCommands();
			else if( command =="exit" or command =="quit" or command =="q") break;
			else if(command == "debug")  DEBUG = (key=="true");
			else cout<<"Invalid command !!!"<<endl;
		}
		catch(exception &ex)
		{
			cout<<ex.what()<<endl;
		}

	}
	return 0;
}
//=====================================================================
AVL::AVL()
{
	this->root=NULL;
}
//=====================================================================
AVL::~AVL()
{
	//Keep removing the root of the tree untill it becomes empty
	while(root)
		remove(root,root->key);
}
//=====================================================================
// Return the root of the tree
Node* AVL::getRoot()
{
	return root;
}
//=====================================================================
//Insert recursively the key in the tree/subtree rooted at ptr
void AVL::insert(Node *ptr, int key)
{
	if (root == nullptr)	root = new Node(key);
	else if(key < ptr->key)		// if the key of new node is smaller than the key of current node
	{
		if (ptr->left ==nullptr)
		{
		    	ptr->left = new Node(key);
		    	ptr->left->parent = ptr;
		}
		else
			insert(ptr->left,key); //Recursively insert the new node in left subtree
	}
	else if(key > ptr->key)		// if the key of new node is greater than the key of current node
	{
		if (ptr->right ==nullptr)
		{
		    	ptr->right = new Node(key);
		    	ptr->right->parent = ptr;
		}
		else
			insert(ptr->right ,key); //Recursively insert the new node in right subtree
	}

	else 	// if key alredy exist is the subtree (if the key of new node is equal to the key of current node)
		cerr<<"Duplicate Keys are not allowed !!"<<endl;

	rebalance(ptr);
}
//=====================================================================
//Find the Height of the tree/subtree rooted at ptr
int AVL::height(Node* ptr)
{
	if(ptr==nullptr) return -1; //height of an empty tree/subtree is considered to be -1
	return max(height(ptr->left),height(ptr->right))+1;
}
//=====================================================================
// Find recursively (or iteratively ) the key with minimum in the tree/subtree rooted at ptr
Node* AVL::findMin(Node *ptr)
{
	if(ptr==nullptr) 		return nullptr; 			//if Tree is empty
	else if(ptr->left)		return findMin(ptr->left); 	//recursive approach to find the node with minimum key in the tree
	else					return ptr;					//if reached to the left most node
}
//=====================================================================
// Find recursively (or iteratively ) the key with Maximum in the tree/subtree rooted at ptr
Node* AVL::findMax(Node *ptr)
{
	if(ptr==nullptr) 		return nullptr; 			//if Tree is empty
	else if(ptr->right)		return findMax(ptr->right); //recursive approach to find the node with Maximum key in the tree
	else					return ptr;					//if reached to the right most node
}
//=====================================================================
// Find recursively the node with key value in the tree/subtree rooted at ptr
Node* AVL::find(Node *ptr,int key)
{
	if(ptr==nullptr) 			return nullptr;					//No key found
	else if(key == ptr->key) 	return ptr;						//Key found
	else if(key < ptr->key) 	return find(ptr->left,key);	//search in the left subtree
	else						return find(ptr->right,key);	//search in the right subtree
}
//=========================================================
//Remove recursively the node with key value from the tree/subtree rooted at ptr
void AVL::remove(Node *ptr,int key)
{
	if (ptr == nullptr)				//Node Not Found		
	{
		cout<<"Key not found"<<endl;	//Reached to bottom of the tree but key not found
		return;
	}
	if(key<ptr->key) 						//if key is smaller than the key at root of the tree/subtree
		remove(ptr->left, key);		//then search in left subtree
	else if(key>ptr->key)
		remove(ptr->right, key);	//else search in right subtree
	else
	{
		Node* node = ptr;
		Node* nextLink; 

		if( node->left != nullptr && node->right != nullptr)			//if the node to be deleted has both left and right children
		{
			Node *min = findMin(node->right);//find the node with minimum key in its right subtree 
			node->key = min->key;
			remove(node->right, min->key);	//recursivly remove the minimum node from right subtree
			rebalance(ptr);
			return;
		}	
		else if(node->left == nullptr && node->right == nullptr)		//if the node to be deleted is a Leaf node then remove it
			nextLink = nullptr;					//parent will be connected to nullptr
		else if(node->right == nullptr)			//if the node to be deleted has no right child then remove it and connect its left child to its parent
		{
			node->left->parent = node->parent;	//connect left child to parent			
			nextLink = node->left;				//parent will be connected to left child
		}
		else if(node->left == nullptr)			//if the node to be deleted has no left child then remove it and connect its right child to its parent
		{
			node->right->parent = node->parent;	//connect right child to the parent
			nextLink = node->right;				//parent will be connected to right child
		}
			
		Node* parent=node->parent;				
		if(parent==nullptr)				root = nextLink;			//in the parent is nullptr, then root is going to be deleted, update the root i.e.connect it to next link
		else if(node==parent->left)		parent->left = nextLink;	//if node to be deleted is left child of the parent then updated parents left link
		else							parent->right = nextLink;	//if node to be deleted is right child of the parent then update parents right link 
		if (ptr==node) ptr=nullptr;		//if the node for which the remove is called has been deleted, set it to null ptr so that loadFactor will not generate a seg. fault when called by rebalance
		delete node;		//delete the node
	}
	rebalance(ptr);
}
//===============================================================
//Print recursively the tree/subtree rooted at ptr Inorder fromat
void AVL::printInorder(Node *ptr)
{
	if(ptr!=nullptr)
	{
		printInorder(ptr->left);
		cout<<ptr->key<<" ";
		printInorder(ptr->right);
	}
}
//===============================================================
//Print a 2D Tree
void AVL::printTree()
{
	cout<<"Tree:"<<endl;

	int levels = height(this->root);
	int max_nodes = pow(2,levels+1)-1;	
	queue<Node*> myQueue;
	vector<Node*> myVector;
	myQueue.push(this->root);

	//Travers the tree in Breadth First Order and store each node into myVector
	for(int i=0; i<max_nodes; i++)		
	{
		Node* cur = myQueue.front();
		myQueue.pop();
		myVector.push_back(cur);
		if(cur != nullptr)
		{
			myQueue.push(cur->left);
			myQueue.push(cur->right);
		}
		else //For Empty nodes push nullptr(placeholder) in Queue 
		{
			myQueue.push(nullptr);		
			myQueue.push(nullptr);
		}	
	}

	if(max_nodes >0)			//if a non-empty tree
	{
		int *spaces = new int[levels+1];
		spaces[levels]=0;
		int level=0;

		for(int j=levels-1; j>=0; j--)
			spaces[j]=2*spaces[j+1]+1;
		
		for (int i=0; i<max_nodes; i++)
		{
			if(i == (pow(2,level)-1))	// if first node of a level
			{
				cout<<endl<<endl;		// go to next line		
				for(int k=0; k<spaces[level]; k++)	//print sufficient spaces befor printing the first node of a level
				   cout<<"   ";
				
				level++;
			}
			if(myVector[i]!=nullptr)     cout<<std::left<<setw(3)<<myVector[i]->getKey();
			else                         cout<<std::left<<setw(3)<<" ";	
			if(level>1)
				for(int k=0; k<spaces[level-2]; k++)	// print spaces between two nodes on same level
					cout<<"   ";
		}

		cout<<endl;
		for(int i=0; i<pow(levels,2.5); i++)	//Add last line with ----
			cout<<"___";
		
		cout<<endl;
		delete[] spaces;
	}
}
//==================================================

int AVL::balanceFactor(Node *ptr)
{
	 if (ptr == nullptr)
        return 0;

    return height(ptr->left) - height(ptr->right);
}
//==================================================
//==========================================

void AVL::rotateRight(Node *z)
{  
	Node *y = z->left;
    z->left = y->right;

    if (y->right != nullptr)
        y->right->parent = z;

    y->parent = z->parent;

    if (z->parent == nullptr)
    {
        root = y;
    }
    else if (z == z->parent->left)
    {
        z->parent->left = y;
    }
    else
    {
        z->parent->right = y;
    }

    y->right = z;
    z->parent = y;
}  
//==========================================

void AVL::rotateLeft(Node *z)
{  

	Node *y = z->right;
    z->right = y->left;

    if (y->left != nullptr)
        y->left->parent = z;

    y->parent = z->parent;

    if (z->parent == nullptr)
    {
        root = y;
    }
    else if (z == z->parent->left)
    {
        z->parent->left = y;
    }
    else
    {
        z->parent->right = y;
    }

    y->left = z;
    z->parent = y;
}
//============================================

void AVL::rebalance(Node *ptr)
{
	while (ptr != nullptr)
    {

        int bf = balanceFactor(ptr);

        //falready balanced
        if (bf == 0)
        {
            //no further balancing needed
            break;
        }
        //right right heavy
        else if (bf > 1 && balanceFactor(ptr->left) >= 0)
        {
            rotateRight(ptr);
        }
        //left left heavy
        else if (bf < -1 && balanceFactor(ptr->right) <= 0)
        {
            rotateLeft(ptr);
        }
        //right left heavy
        else if (bf > 1 && balanceFactor(ptr->left) < 0)
        {
            rotateLeft(ptr->left);
            rotateRight(ptr);
        }
        //left right heavy
        else if (bf < -1 && balanceFactor(ptr->right) > 0)
        {
            rotateRight(ptr->right);
            rotateLeft(ptr);
        }

        ptr = ptr->parent;

        
    }

}













