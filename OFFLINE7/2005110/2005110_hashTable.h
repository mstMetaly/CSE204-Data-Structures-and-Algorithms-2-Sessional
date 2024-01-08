#include<iostream>
#include<bits/stdc++.h>

using namespace std;

///hashedObj with a given key and value
class hashedObj
{

public:
    string key;
    int value;

    hashedObj()
    {

    }

    hashedObj(string key,int value)
    {
        this->key=key;
        this->value=value;
    }

};

///Separate_Chaining
class SeparateChaining_HashTable
{

public:
    vector<vector<hashedObj>> hashTable;//hashTable to store the hashedObj
    int tableSize;//size of the hashTable

    SeparateChaining_HashTable(int tableSize=0)
    {
        this->tableSize=tableSize;
        // hashTable.resize(tableSize);
    }

    //find returns true if finds a copy of the given hashedObj
    int Find(int index,string key)
    {
        int prob=1;

        for (auto& item : hashTable[index])
        {
            string data = item.key;
            if(data==key)
                return prob;
            prob++;

        }

        return prob;

    }

    //Insert inserts a hashedObj in the hashTable
    int Insert(int index,hashedObj& obj)
    {
        int collision=0;
        if(hashTable[index].size()==0)
        {
            hashTable[index].push_back(obj);
            return collision;
        }
        else if(hashTable[index].size()>0)
        {
            hashTable[index].push_back(obj);
            collision++;
            return collision;
        }
    }

    void Delete(int index,hashedObj& obj)
    {
        auto& items = hashTable[index];

        for (auto it = items.begin(); it != items.end(); ++it)
        {
            if (it->key == obj.key)
            {
                it->key = "deleted";
                it->value = -1;
                break;
            }
        }
    }


};

///Double Hashing
//Probing hashedObj

class prob_hashedObj
{
public:
    string key;
    int value;

    prob_hashedObj()
    {
        key=" ";
        value = (-1);
    }

    prob_hashedObj(string key,int value)
    {
        this->key=key;
        this->value=value;

    }

};

///DoubleHashing HashTable

class DoubleHashing_HashTable
{

public:
    vector<prob_hashedObj> hashTable;//hashTable to store the hashedObj
    int tableSize;//size of the hashTable

    DoubleHashing_HashTable(int tableSize=0)
    {
        this->tableSize=tableSize;

    }

    //doubleHash returns an index for the given key
    int doubleHash(int hashValue,int auxHashValue,int i)
    {
        ///doubleHash(k, i) = (Hash(k) + i × auxHash(k)) % N
        int index= (hashValue + i * auxHashValue) % tableSize;
        return index;

    }

    //Find returns index and prob
    pair<int,int>Find(int hashValue,int auxHashValue,string key)
    {
        int index=hashValue;
        int prob=1;

        if(hashTable[index].key == key)
        {
            return {index,prob};
        }
        else
        {
            for(int i=1; i<tableSize; i++)
            {
                index=doubleHash(hashValue,auxHashValue,i);
                if(hashTable[index].key==key)
                    return {index,prob};
                prob++;
            }
        }

        return {-1,prob};
    }


    //Insert a hashedObj into the hashTable
    int Insert(int hashValue,int auxHashValue,prob_hashedObj& obj)
    {
        int index=hashValue;

        int collision=0;
        if(hashTable[index].value == -1)
        {
            hashTable[index] = obj;
            return collision;
        }
        else
        {
            int slot;
            for(int i=1; i<tableSize; i++)
            {
                slot=doubleHash(hashValue,auxHashValue,i);
                if(hashTable[slot].value == -1)
                {
                    hashTable[slot]= obj;
                    return collision;
                }
                collision++;

            }
            return collision;

        }

    }

    void Delete(int hashValue,int auxHashValue,string key)
    {
        int index=Find(hashValue,auxHashValue,key).first;
        if(index!= -1)
        {
            hashTable[index].key="Deleted";
            hashTable[index].value=-1;
        }
    }

};



//Custom Hashing
class CustomHashing_HashTable
{
public:

    vector<prob_hashedObj> hashTable;//hashTable to store the hashedObj
    int tableSize;//size of the hashTable


    CustomHashing_HashTable(int tableSize=0)
    {
        this->tableSize=tableSize;
    }

    ///customHash returns an index for the given key
    int customHash(int hashValue,int auxHashValue,int i)
    {
        // customHash(k, i) = (Hash(k) + C1 × i × auxHash(k) + C2 × i^2) % N
        int C1=11,C2=13;
        int index= (hashValue + (C1 * i * auxHashValue) + (C2 * i * i)) % tableSize;
        return index;

    }

    ///Find returns index and prob
    pair<int,int>Find(int hashValue,int auxHashValue,string key)
    {
        int index=hashValue;
        int prob=1;

        if(hashTable[index].key== key)
        {
            return {index,prob};
        }
        else
        {
            for(int i=1; i<hashTable.size(); i++)
            {
                index=customHash(hashValue,auxHashValue,i);
                if(hashTable[index].key==key)
                    return {index,prob};
                prob++;
            }
        }

        return {-1,prob};
    }


    ///Insert a hashedObj into the hashTable
    int Insert(int hashValue,int auxHashValue,prob_hashedObj& obj)
    {
        int collision=0;
        int index=hashValue;

        if(hashTable[index].value== -1)
        {
            hashTable[index]=obj;
            return collision;
        }
        else
        {
            for(int i=0; i<hashTable.size(); i++)
            {
                int slot=customHash(hashValue,auxHashValue,i);
                if(hashTable[slot].value == -1)
                {
                    hashTable[slot]=obj;
                    return collision;
                }

                collision++;

            }

        }

        return collision;

    }

    void Delete(int hashValue,int auxHashValue,string key)
    {
        int index=Find(hashValue,auxHashValue,key).first;
        if(index!= -1)
        {
            hashTable[index].key="Deleted";
            hashTable[index].value=-1;
        }
    }


};


