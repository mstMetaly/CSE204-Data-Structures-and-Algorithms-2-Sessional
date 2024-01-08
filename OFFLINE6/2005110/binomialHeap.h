#include<iostream>
#include<vector>
#include<queue>

using namespace std;

//node
class Node
{
public:
    int key,degree;
    Node* parent;
    Node* child;
    Node* sibling;

    Node()
    {
        key=0;
        degree=0;
        parent=NULL;
        child=NULL;
        sibling=NULL;
    }

    Node(int key)
    {
        this->key=key;
        degree=0;
        parent=NULL;
        child=NULL;
        sibling=NULL;
    }

    Node(Node& node)
    {
        this->key=node.key;
        this->degree=node.degree;
        this->parent=node.parent;
        this->child=node.child;
        this->sibling=node.sibling;
    }
};

//
class binomial_heap
{
public:
    vector<Node*>rootList;

    //make tree 2 as a child of tree 1
    void merge_binomial_tree(Node* tree1,Node* tree2)
    {
        tree2->parent=tree1;
        tree2->sibling=tree1->child;
        tree1->child=tree2;
        tree1->degree+=1;

    }

    //merge a new binomial heap with existing binomial heap
    vector<Node*> merge_binomial_heap(binomial_heap& h)
    {
        auto itr1 = this->rootList.begin();
        auto h_itr2 = h.rootList.begin();

        vector<Node*>tempList;

        while(itr1 != rootList.end() && h_itr2 != h.rootList.end())
        {
            if((*itr1)->degree < (*h_itr2)->degree)
            {
                tempList.push_back(*itr1);
                itr1++;
            }
            else
            {
                tempList.push_back(*h_itr2);
                h_itr2++;
            }
        }
        if(itr1 != rootList.end())
        {
            while(itr1 != rootList.end())
            {
                tempList.push_back(*itr1);
                itr1++;
            }
        }
        else
        {
            while(h_itr2 != h.rootList.end())
            {
                tempList.push_back(*h_itr2);
                h_itr2++;
            }
        }
        return tempList;
    }


    //Union : Merges two binomial heaps
    //takes O (log n )
    void Union(binomial_heap& h)
    {

        this->rootList= merge_binomial_heap(h);
        int current=0;
        int length=rootList.size();


        auto itr=rootList.begin();

        while(current+1 < length)
        {
            if(rootList[current]->degree != rootList[current+1]->degree)
            {
                current++;
                itr++;
            }
            else if(current+2 < length && rootList[current+2]->degree == rootList[current]->degree)
            {
                current++;
                itr++;
            }
            else
            {
                if(rootList[current]->key > rootList[current+1]->key)
                {
                    merge_binomial_tree(rootList[current+1], rootList[current]);

                    itr = rootList.erase(itr);
                }
                else
                {
                    merge_binomial_tree(rootList[current], rootList[current+1]);
                    itr++;
                    itr = rootList.erase(itr);
                    itr--;
                }

                length--;
            }
        }


    }


    // Insert : Inserts a new node in the heap
    //takes O ( log n)
    void Insert(int data)
    {
        Node* newNode=new Node(data);

        binomial_heap heap;

        heap.rootList.push_back(newNode);
        Union(heap);
    }


    // Find-Min : Returns the node having the minimum key
    //takes O ( log n)
    Node* Find_Min()
    {
        int length=rootList.size();
        Node* y=rootList[0];

        int minimum=rootList[0]->key;

        for(int i=1; i<length; i++)
        {
            if(rootList[i]->key < minimum)
            {
                minimum=rootList[i]->key;
                y=rootList[i];
            }
        }

        return y;

    }

    // Extract-Min : Returns the node having the minimum key and deletes it from the heap
    //takes O ( log n)

    Node* Extract_Min()
    {

        Node* minNode=Find_Min();
        cout << "min Node: "<< minNode->key << endl;

        if(rootList.size()==1 && rootList[0]->degree==0)
        {
            Node* temp=rootList[0];
            rootList.clear();
            return temp;
        }

        if(rootList.size()==1 && rootList[0]->degree==1)
        {
            Node* temp=rootList[0]->child;
            Node* temp2=rootList[0];
            rootList.clear();
            rootList.push_back(temp);
            return temp2;
        }

        binomial_heap heap;

        vector<Node*>temp_heaprootList;

        temp_heaprootList.push_back(minNode->child);

        Node* temp=minNode->child->sibling;

        while(temp!=NULL)
        {
            if(temp->parent->key==minNode->key)
            {
                temp_heaprootList.push_back(temp);
            }
            temp=temp->sibling;
        }

        for(int i=temp_heaprootList.size()-1; i>=0; i--)
        {
            heap.rootList.push_back(temp_heaprootList[i]);
        }


        vector<Node*>new_rootList;

        int len=rootList.size();

        for(int i=0; i<len; i++)
        {
            if(rootList[i]->key != minNode->key)
            {
                new_rootList.push_back(rootList[i]);
            }
        }

        rootList.clear();

        rootList=new_rootList;

        Union(heap);
        return minNode;

    }

    //Print : Prints the level order traversal with level no. of each of the binomial trees in the heap

    void Print(ofstream& out)
    {
        cout << "Printing Binomial Heap..."<< endl;
        out << "Printing Binomial Heap..."<< endl;

        int len = rootList.size();
        for(int i=0; i<len; i++)
        {
            cout << "Binomial Tree, B"<< rootList[i]->degree << endl;
            out << "Binomial Tree, B"<< rootList[i]->degree << endl;
            print_binomial_tree(rootList[i],out);
        }


    }

    void print_binomial_tree(Node* root,ofstream& out)
    {
        queue<Node*>q;
        q.push(root);

        int level=0;
        cout << "Level "<< level<<" : "<< root->key<< endl;
        out << "Level "<< level<<" : "<< root->key<< endl;
        level++;

        Node* rootTemp= q.front();
        q.pop();
        Node* siblingTemp=rootTemp->child;

        cout << "Level "<< level<<" : ";
        out << "Level "<< level<<" : ";
        while(siblingTemp!=NULL)
        {
            q.push(siblingTemp);
            cout << siblingTemp->key << "  ";
            out << siblingTemp->key << "  ";
            siblingTemp=siblingTemp->sibling;
        }

        q.push(nullptr);
        cout << endl;
        level++;
        cout <<"Level "<< level<<" : ";
        out <<"Level "<< level<<" : ";

        while(!q.empty())
        {
            Node* temp=q.front();
            q.pop();
            if(temp)
            {
                if(temp->child)
                {
                    Node* test=temp->child;

                    while(test!=NULL)
                    {
                        q.push(test);
                        cout << test->key << "  ";
                        out << test->key << "  ";
                        test=test->sibling;
                    }
                }
            }
            else if(temp==nullptr)
            {
                cout << endl;
                level++;
                if(level > rootTemp->degree)
                    break;
                cout <<"Level "<< level<< " : ";
                out <<"Level "<< level<< " : ";
                q.push(nullptr);

            }


        }



    }



};

