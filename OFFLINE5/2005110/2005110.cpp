#include<iostream>
#include<fstream>
#include<sys/time.h>
#include<iomanip>

using namespace std;

template<class T>
class Node
{
public:
    T value;
    Node* left;
    Node* right;

    Node()
    {
        left=NULL;
        right=NULL;
    }

    Node(T value)
    {
        this->value=value;
        left=NULL;
        right=NULL;
    }
};


template<class T>
class AVL_Tree
{
public:
    Node<T>* root;

    AVL_Tree()
    {
        this->root=NULL;
    }

    //Insert (I) followed by an integer denoting the value of the node to be inserted
    Node<T>* Insert(Node<T>* root,T data)
    {
        Node<T>* newNode=new Node<T>(data);
        if(root==NULL)
            return newNode;
        else if(data<root->value)
            root->left=Insert(root->left,data);
        else if(data>root->value)
            root->right=Insert(root->right,data);

        //rotation

        int balanceFactor=getBalanceFactor(root);

        if(balanceFactor<-1 && data>root->right->value)//LL
            return leftRotation(root);
        else if(balanceFactor>1 && data<root->left->value)//RR
            return rightRotation(root);
        else if(balanceFactor>1 && data>root->left->value)//LR
        {
            root->left=leftRotation(root->left);
            return rightRotation(root);
        }

        else if(balanceFactor<-1 && data <root->right->value)//RL
        {
            root->right=rightRotation(root->right);
            return leftRotation(root);
        }

        this->root=root;
        return root;

    }

    //get balance factor for a  node
    int getBalanceFactor(Node<T>* root)
    {
        int diff=height(root->left)-height(root->right);
        return diff;
    }

    //get height for a tree
    int height(Node<T>* root)
    {
        if(root==NULL)
            return 0;
        return max(height(root->left),height(root->right))+1;
    }

    //left rotation  for balancing
    Node<T>* leftRotation(Node<T>* y)
    {
        Node<T>* x=y->right;
        Node<T>* temp=x->left;
        x->left=y;
        y->right=temp;
        return x;
    }

    //right rotation for balancing
    Node<T>* rightRotation(Node<T>* y)
    {
        Node<T>* x=y->left;
        Node<T>* temp=x->right;
        x->right=y;
        y->left=temp;
        return x;
    }

    //Delete (D) followed by an integer denoting the value of the node to be delete for
    void Delete(T data)
    {
        this->root=deleteNode(root,data);
    }

    Node<T>* deleteNode(Node<T>* root,T data)
    {
        if(root==NULL)
            return root;
        if(data<root->value)
            root->left=deleteNode(root->left,data);
        else if(data>root->value)
            root->right=deleteNode(root->right,data);
        else
        {
            if(root->left==NULL && root->right==NULL)
                return NULL;
            else if(root->right==NULL)
            {
                Node<T>* temp=root->left;
                delete(root);
                return temp;
            }

            else if(root->left==NULL)
            {
                Node<T>* temp=root->right;
                delete(root);
                return temp;
            }
            else
            {
                Node<T>* temp=inorderSuccessorNode(root->right);
                root->value=temp->value;
                root->right=deleteNode(root->right,temp->value);
            }

        }

        ///Rotation
        int balanceFactor=getBalanceFactor(root);

        if(balanceFactor<-1 && getBalanceFactor(root->right)<=0)//LL
            return leftRotation(root);
        else if(balanceFactor>1 && getBalanceFactor(root->left)>=0)//RR
            return rightRotation(root);
        else if(balanceFactor>1 && getBalanceFactor(root->left)==-1)//LR
        {
            root->left=leftRotation(root->left);
            return rightRotation(root);
        }

        else if(balanceFactor<-1 && getBalanceFactor(root->right)==1)//RL
        {
            root->right=rightRotation(root->right);
            return leftRotation(root);
        }

        this->root=root;
        return root;
    }

    //Inorder Successor
    Node<T>* inorderSuccessorNode(Node<T>* root)
    {
        Node<T>* current=root;
        while(current->left!=NULL)
            current=current->left;
        return current;

    }

    //Find (F) followed by an integer denoting the value of the node to be searched for
    bool Find(T data)
    {
        if(searchNode(root,data))
            return true;
        return false;
    }

    bool searchNode(Node<T>* root,T data)
    {
        if(root==NULL)
            return false;
        if(root->value==data)
            return true;
        else if(data<root->value)
            searchNode(root->left,data);
        else if(data>root->value)
            searchNode(root->right,data);
    }

    //in order  Traversal (T).
    void Traversal(Node<T>* root,ofstream& out)
    {
        if(root==NULL)
        {
            return ;
        }

        Traversal(root->left,out);
        cout << root->value<< "  ";
        out << root->value<< "  ";
        Traversal(root->right,out);

    }

    void print(Node<T>* root,ofstream& out)
    {
        if(root==NULL )
        {
            return;
        }

        cout << root->value<< " " ;
        out << root->value<< " " ;
        if(root->left==NULL && root->right==NULL)
            return;
        cout << "(";
        out << "(";
        print(root->left,out);
        cout << ",";
        out << ",";
        print(root->right,out);
        cout << ") ";
        out << ") ";
    }

};





int main()
{
    ifstream infile;
    ofstream outfile;
    ofstream reportFile;

    infile.open("in.txt");
    outfile.open("out_avl.txt");
    reportFile.open(" report_avl.txt");

    AVL_Tree<int> avl;

    double insert_diff=0,dlt_diff=0,find_diff=0,trav_diff=0;
    clock_t start,End;

    while(!infile.eof())
    {

        char order;
        int parameter;

        infile >> order;
        if(!infile.eof())
        {
            switch (order)
            {

            case 'I':
            {
                infile>>parameter;

                start=clock();
                avl.root=avl.Insert(avl.root,parameter);
                End=clock();

                avl.print(avl.root,outfile);
                cout << endl;
                outfile<<endl;

                insert_diff += ((double(End - start) / double(CLOCKS_PER_SEC))*1000);
                break;
            }

            case 'D':
            {
                infile>>parameter;

                start=clock();
                avl.Delete(parameter);
                End=clock();

                avl.print(avl.root,outfile);
                cout <<endl;
                outfile <<endl;

                dlt_diff+= ((double(End - start) / double(CLOCKS_PER_SEC))*1000);
                break;
            }
            case 'F':
            {
                infile>>parameter;

                start=clock();
                if(avl.Find(parameter))
                {
                    cout << "found"<< endl;
                    outfile << "found"<< endl;
                }
                else
                {
                    cout << "not found"<< endl;
                    outfile << "not found"<< endl;
                }

                End=clock();

                find_diff+= ((double(End - start) / double(CLOCKS_PER_SEC))*1000);
                break;
            }


            case 'T':
            {
                start=clock();
                avl.Traversal(avl.root,outfile);
                cout <<endl;
                outfile<<endl;
                End=clock();

                trav_diff+= ((double(End - start) / double(CLOCKS_PER_SEC))*1000);
                break;
            }
            }
        }
    }

    reportFile << "operation time(ms)"<<endl;
    reportFile<<"insert   "<< fixed<< insert_diff<< setprecision(8)<<endl;
    reportFile<<"delete   "<< fixed<< dlt_diff<< setprecision(8)<<endl;
    reportFile<<"search   "<< fixed<< find_diff<< setprecision(8)<<endl;
    reportFile<<"trav   "<< fixed<< trav_diff<< setprecision(8)<<endl;
    reportFile<<"total   "<<fixed << (insert_diff+dlt_diff+find_diff+trav_diff)<<setprecision(8)<<endl;

    infile.close();
    reportFile.close();

    return 0;
}
