#include<iostream>
#include<bits/stdc++.h>
#include "2005110_hashTable.h"

using namespace std;


///HashTable
class HashTable
{

public:
    int N,hashType;
    SeparateChaining_HashTable table_separateH;
    DoubleHashing_HashTable table_doubleH;
    CustomHashing_HashTable table_customH;
    string type;

    ///constructor
    HashTable(int N,string s,int hashType)
    {
        this->N=findNearestPrime(N);
        this->hashType=hashType;
        this->type=s;

        if(s=="SeparateChaining")
        {
            table_separateH.tableSize=this->N;

            for(int i=0; i<table_separateH.tableSize; i++)
            {
                vector<hashedObj>v;
                table_separateH.hashTable.push_back(v);
            }

        }

        else if(s=="DoubleHashing")
        {
            table_doubleH.tableSize=this->N;
            for(int i=0; i<table_doubleH.tableSize; i++)
            {
                prob_hashedObj obj(" ",-1);
                table_doubleH.hashTable.push_back(obj);
            }

        }

        else if(s=="CustomHashing")
        {
            table_customH.tableSize=this->N;
            for(int i=0; i<table_customH.tableSize; i++)
            {
                prob_hashedObj obj(" ",-1);
                table_customH.hashTable.push_back(obj);
            }
        }

    }

    ///returns the nearest prime number for the table size
    int findNearestPrime(int n)
    {
        while (true)
        {
            if (isPrime(n))
            {
                return n; // Return the first prime number found
            }
            ++n; // Move to the next number
        }
    }

    ///check a number is prime or not
    bool isPrime(int number)
    {
        if (number <= 1)
        {
            return false; // Numbers less than or equal to 1 are not prime
        }

        for (int i = 2; i * i <= number; ++i)
        {
            if (number % i == 0)
            {
                return false; // If divisible by any number from 2 to sqrt(number), not prime
            }
        }

        return true; // If not divisible by any number, prime
    }


    /// polinomial hash function
    int Hash1(string s)
    {
        int p = 31;
        long long p_pow = 1;
        const int m = 1e9 + 9;

        long long hashValue = 0;
        for (char c : s)
        {
            hashValue = (hashValue + (c - 'a' + 1) * p_pow) % m;
            p_pow = (p_pow * p) % m;
        }
        return hashValue%N;
    }

    int Hash2(string s)
    {

        int p = 37;
        long long p_pow = 1;
        const int m = 1e9 + 9;

        long long hashValue = 0;
        for (char c : s)
        {
            hashValue = (hashValue + (c - 'a' + 1) * p_pow) % m;
            p_pow = (p_pow * p) % m;
        }
        return hashValue%N;
    }


    typedef long long ll;
    int auxHash(string key)
    {
        long long hashVal = 5381;
        int c;

        for(char c: key)
            hashVal = (((hashVal << 5) + hashVal) + c)%(ll)(1e9+7); /* hash * 33 + c */

        return hashVal%N;
    }

    ///Separate Insert
    int Separate_Insert(hashedObj& obj)
    {
        int hashValue;
        if(hashType==1)
        {
            hashValue=Hash1(obj.key);
        }
        else if(hashType==2)
        {
            hashValue=Hash2(obj.key);
        }

        return table_separateH.Insert(hashValue,obj);

    }

    ///Probing Insert
    int Probing_Insert(prob_hashedObj& obj)
    {
        int hashValue;

        if(hashType==1)
        {
            hashValue=Hash1(obj.key);
        }
        else if(hashType==2)
        {
            hashValue=Hash2(obj.key);
        }

        int auxHashValue=auxHash(obj.key);

        if(type=="DoubleHashing")
        {
            int col=table_doubleH.Insert(hashValue,auxHashValue,obj);
            return col;
        }
        else if(type=="CustomHashing")
        {
            return table_customH.Insert(hashValue,auxHashValue,obj);
        }

    }

    ///Separate Find -returns a pair of index and a boolean

    int Separate_Find(hashedObj &obj)
    {
        int hashValue;
        if(hashType==1)
            hashValue=Hash1(obj.key);
        else if(hashType==2)
            hashValue=Hash2(obj.key);

        return table_separateH.Find(hashValue,obj.key);

    }

    ///Probing Find
    pair<int,int> Probing_Find(prob_hashedObj &obj)
    {
        int hashValue;
        if(hashType==1)
            hashValue=Hash1(obj.key);
        else if(hashType==2)
            hashValue=Hash2(obj.key);

        int auxHashValue=auxHash(obj.key);

        if(type=="DoubleHashing")
        {
            return table_doubleH.Find(hashValue,auxHashValue,obj.key);
        }

        else if(type=="CustomHashing")
        {
            return table_customH.Find(hashValue,auxHashValue,obj.key);
        }


    }


    ///Delete
    void Separate_Delete(hashedObj& obj)
    {
        int hashValue;
        if(hashType==1)
        {
            hashValue=Hash1(obj.key);
        }
        else if(hashType==2)
        {
            hashValue=Hash2(obj.key);
        }

        table_separateH.Delete(hashValue,obj);
    }

    ///Probing delete
    void Probing_Delete(prob_hashedObj &obj)
    {

        int hashValue;
        if(hashType==1)
        {
            hashValue=Hash1(obj.key);
        }
        else if(hashType==2)
        {
            hashValue=Hash2(obj.key);
        }

        int auxHashValue=auxHash(obj.key);

        if(type=="DoubleHashing")
        {
            table_doubleH.Delete(hashValue,auxHashValue,obj.key);
        }

        else if(type=="CustomHashing")
        {
            table_customH.Delete(hashValue,auxHashValue,obj.key);
        }


    }


};


//random 5 to 10 length String Generation
void generateString(int n,vector<string>& v)
{
    /// srand(clock());

    unordered_set<string> uniqueStrings; // To keep track of unique strings

    for (int i = 0; i < n; i++)
    {
        int randomNumber = rand();
        int len = 5 + (randomNumber % 6);

        string s(len, '*');
        for (int j = 0; j < len; j++)
        {
            int r = rand() % 26;
            char ch = 'a' + r;
            s[j] = ch;
        }

        // Check if the generated string is unique
        if (uniqueStrings.find(s) == uniqueStrings.end())
        {
            uniqueStrings.insert(s);
            v.push_back(s);
        }
        else
        {
            i--;
        }
    }

}


int main()
{

    ifstream infile;
    ofstream outfile;

    outfile.open("report.txt");

    vector<string>v;
    generateString(10000,v);

    ///int N,string s,int hashType
    int N=20000;
    cout << "HashTable Size: "<< N << endl<< endl;
    outfile << "HashTable Size: "<< N << endl<< endl;

    int hashtype1=1,hashtype2=2;

    int totalCollision=0;


    cout <<"------Separate Hashing------"<<endl;
    outfile <<"------Separate Hashing------"<<endl;

    for(int i=1; i<=2; i++)
    {
        cout << "   Hash" << i <<  endl;
        outfile << "   Hash" << i <<  endl;

        HashTable tableObj(N,"SeparateChaining",i);

        int value=1;

        for(int i=0; i<v.size(); i++)
        {
            hashedObj obj(v[i],value);

            int col=tableObj.Separate_Insert(obj);

            totalCollision+=col;

            value++;
        }

        cout << "Total Collision :"<< totalCollision << "              ";
        outfile << "Total Collision :"<< totalCollision << "              ";

        int totalProb=0;
        int val=1;

        for(int i=0; i<1000; i++)
        {
            int randVal=rand();
            randVal=randVal%1000;

            hashedObj ob(v[randVal],val);

            int probVal= (tableObj.Separate_Find(ob));

            totalProb+=probVal;
            val++;

        }

        cout << "Average Prob: "<< totalProb/1000 <<endl<< endl;
        outfile << "Average Prob: "<< totalProb/1000 <<endl<< endl;

    }


    cout <<endl<< "------DoubleHashing:------"<< endl;

    outfile <<endl<< "------DoubleHashing:------"<< endl;


    for(int i=1; i<=2; i++)
    {
        cout << "   Hash" << i <<  endl;
        outfile << "   Hash" << i <<  endl;
        HashTable tableObj(N,"DoubleHashing",i);

        int value=1;

        for(int i=0; i<v.size(); i++)
        {
            prob_hashedObj obj(v[i],value);

            int col=tableObj.Probing_Insert(obj);
            totalCollision+=col;

            value++;
        }

        cout << "Total Collison :"<< totalCollision <<  "              ";
        outfile << "Total Collison :"<< totalCollision <<  "              ";

        int totalProb=0;
        int val=1;

        for(int i=0; i<1000; i++)
        {
            int randVal=rand();
            randVal=randVal%1000;

            prob_hashedObj ob(v[randVal],val);


            int probVal= (tableObj.Probing_Find(ob)).second;

            totalProb+=probVal;
            val++;

        }

        cout << "Average Prob: "<< totalProb/1000 << endl<<endl;
        outfile << "Average Prob: "<< totalProb/1000 << endl<<endl;

    }

    cout <<endl<< "------Custom Hashing------"<<endl;
    outfile <<endl<< "------Custom Hashing------"<<endl;
    for(int i=1; i<=2; i++)
    {
        cout << "   Hash" << i <<  endl;
        outfile << "   Hash" << i <<  endl;
        HashTable tableObj(N,"CustomHashing",i);

        int value=1;

        for(int i=0; i<v.size(); i++)
        {
            prob_hashedObj obj(v[i],value);

            int col=tableObj.Probing_Insert(obj);
            totalCollision+=col;

            value++;
        }

        cout << "Total Collison :"<< totalCollision << "              " ;
        outfile << "Total Collison :"<< totalCollision << "              " ;

        int totalProb=0;
        int val=1;

        for(int i=0; i<1000; i++)
        {
            int randVal=rand();
            randVal=randVal%1000;

            prob_hashedObj ob(v[randVal],val);


            int probVal= (tableObj.Probing_Find(ob)).second;

            totalProb+=probVal;
            val++;

        }

        cout << "Average Prob: "<< totalProb/1000 << endl<<endl;
        outfile << "Average Prob: "<< totalProb/1000 << endl<<endl;

    }



    return 0;

}

