#include<iostream>
#include<fstream>
#include<sstream>
#include "binomialHeap.h"

using namespace std;

int main()
{
    binomial_heap h;

    ifstream infile;
    ofstream outfile;

    infile.open("input.txt");
    outfile.open("output.txt");

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
                infile >> parameter;
                h.Insert(parameter);
                break;
            }

            case 'F':
            {
                Node* minNode=h.Find_Min();
                cout << "Find-Min returned "<< minNode->key << endl;
                outfile <<  "Find-Min returned "<< minNode->key << endl;
                break;
            }
            case 'E':
            {
                Node* extractNode=h.Extract_Min();
                cout << "Extract-Min returned "<< extractNode->key << endl;
                outfile <<  "Extract-Min returned "<< extractNode->key << endl;
                break;
            }
            case 'U':
            {
                string line;
                getline(infile, line);

                vector<int> numbers;
                istringstream iss(line);

                int num;
                while (iss >> num)
                {
                    numbers.push_back(num);
                }

                binomial_heap heap;
                for(int x: numbers)
                {
                    heap.Insert(x);
                }

                h.Union(heap);

                break;
            }



            case 'P':
            {
                h.Print(outfile);
                break;
            }


            }
        }

    }


    return 0;
}

