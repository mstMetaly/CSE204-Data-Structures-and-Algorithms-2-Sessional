#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>

using namespace std;

//make function assigns a vertex to it's parent itself
//keep initial size 1 for a vertex

void make(int v,int parent[],int setSize[])
{
    parent[v]=v;
    setSize[v]=1;
}

//finds a parent for a vertex using path compression

int Find(int v,int parent[])
{
    if(parent[v]==v)
        return v;
    return parent[v]=Find(parent[v],parent);
}

//makes two disjoint set into a single set using union by size

void Union(int a,int b,int parent[],int setSize[])
{

    if(setSize[a]<setSize[b])
        swap(a,b);
    parent[b]=a;
    setSize[a]+=setSize[b];

}

bool cmp(pair<double,pair<int,int>>&wtEdge1,pair<double,pair<int,int>>& wtEdge2)
{
    return wtEdge1.first < wtEdge2.first;
}

int krushkalAlgo(vector<pair<double,pair<int,int>>>& wtEdges,int parent[],int setSize[],vector<pair<int,int>>&mst)
{
    double totalWeight=0;

    sort(wtEdges.begin(),wtEdges.end(),cmp);///sorting time complexity: O(E logE)

    //iterate through all the edges ,it would take O ( E alpha(V))
    //where alpha (V) regarded as constant due to optimization of find and union function

    for(auto edge : wtEdges)
    {
        int u=edge.second.first;
        int v=edge.second.second;
        double d=edge.first;

        //for each edge, the find and union operation take nearly constant time
        // due to path compression and union by size techniques

        int u_parent=Find(u,parent);
        int v_parent=Find(v,parent);
        if(u_parent!=v_parent)
        {
            Union(u_parent,v_parent,parent,setSize);
            mst.push_back(make_pair(u,v));
            totalWeight+=d;
        }
    }

    return totalWeight;

}

int main()
{
    ifstream infile;
    ofstream outfile;

    infile.open("input.txt");
    outfile.open("krushkal.txt");

    int n,m;
    infile >> n >> m;

    int parent[n];
    int setSize[n];
    double totalWeight;

    vector<pair<double,pair<int,int>>>edges;
    vector<pair<int,int>>mst;

    for(int i=0; i<n; i++)
    {
        make(i,parent,setSize);
    }


    for(int i=1; i<=m; i++)
    {
        int u,v;
        double d;
        infile >> u >> v >> d ;

        pair<int,int>edge=make_pair(u,v);
        edges.push_back(make_pair(d,edge));
    }


    totalWeight=krushkalAlgo(edges,parent,setSize,mst);

    cout << "Krushkal's Algorithm ------"<< endl;
    cout << "TotalWeight : "<< totalWeight << endl;

    outfile << "Krushkal's Algorithm ------"<< endl;
    outfile << "TotalWeight : "<< totalWeight << endl;

    for(auto edge : mst)
    {
        cout << edge.first << "   "<< edge.second << endl;
        outfile << edge.first << "   "<< edge.second << endl;
    }

    return 0;

}
